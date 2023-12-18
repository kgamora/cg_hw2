#include "Window.h"

#include <QMouseEvent>
#include <QLabel>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QVBoxLayout>
#include <QScreen>

#include <array>
#include <iostream>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <tinygltf/tiny_gltf.h>

namespace
{

constexpr std::array<GLfloat, 40u> vertices = {
//	COORDS			 // COLORS		// TexCoord
	-0.5f, -.3f,  0.5f, 0.f, 1.f, 0.f, 0.0f, 0.0f,
	-0.5f, -.3f, -0.5f, 0.f, 0.f, 1.f, 5.0f, 0.0f,
	 0.5f, -.3f, -0.5f, 1.f, 0.f, 1.f, 0.0f, 0.0f,
	 0.5f, -.3f,  0.5f, 0.33f, 0.33f, 0.33f, 5.0f, 0.0f,
	 0.0f, 0.7f,  0.0f, 1.f, 0.f, 0.f, 2.5f, 5.0f,
};
constexpr std::array<GLuint, 18u> indices = {
	0, 1, 2,
	0, 2, 3,
	1, 0, 4,
	2, 1, 4,
	3, 2, 4,
	0, 3, 4
};

}// namespace

Window::Window() noexcept
{
	const auto formatFPS = [](const auto value) {
		return QString("FPS: %1").arg(QString::number(value));
	};

	auto fps = new QLabel(formatFPS(0), this);
	fps->setStyleSheet("QLabel { color : white; }");

	auto layout = new QVBoxLayout();
	layout->addWidget(fps, 1);

	setLayout(layout);

	timer_.start();

	connect(this, &Window::updateUI, [=] {
		fps->setText(formatFPS(ui_.fps));
	});
}

Window::~Window()
{
	{
		// Free resources with context bounded.
		const auto guard = bindContext();
		texture_.reset();
		program_.reset();
	}
}

bool loadModel(tinygltf::Model &model, const char *filename) {
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;

	bool res = loader.LoadBinaryFromFile(&model, &err, &warn, filename);
	if (!warn.empty()) {
		std::cout << "WARN: " << warn << std::endl;
	}

	if (!err.empty()) {
		std::cout << "ERR: " << err << std::endl;
	}

	if (!res)
		std::cout << "Failed to load glTF: " << filename << std::endl;
	else
		std::cout << "Loaded glTF: " << filename << std::endl;

	return res;
}

void Window::onInit()
{
	// Configure shaders
	program_ = std::make_unique<QOpenGLShaderProgram>(this);
	program_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/diffuse.vs");
	program_->addShaderFromSourceFile(QOpenGLShader::Fragment,
									  ":/Shaders/diffuse.fs");

	tinygltf::Model model;
	std::string filename = "/home/constantine/computer_graphics/cg_hw2/src/App/Models/chess.glb";
	if (!loadModel(model, filename.c_str())) return;

	program_->link();

	// Create VAO object
	vao_.create();
	vao_.bind();

	// Create VBO
	vbo_.create();
	vbo_.bind();
	vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
	vbo_.allocate(vertices.data(), static_cast<int>(vertices.size() * sizeof(GLfloat)));

	// Create IBO
	ibo_.create();
	ibo_.bind();
	ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
	ibo_.allocate(indices.data(), static_cast<int>(indices.size() * sizeof(GLuint)));

	texture_ = std::make_unique<QOpenGLTexture>(QImage(":/Textures/voronoi.png"));
	texture_->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
	texture_->setWrapMode(QOpenGLTexture::WrapMode::Repeat);

	// Bind attributes
	program_->bind();

	program_->enableAttributeArray(0);
	program_->setAttributeBuffer(0, GL_FLOAT, 0, 3, static_cast<int>(8 * sizeof(GLfloat)));

	program_->enableAttributeArray(1);
	program_->setAttributeBuffer(1, GL_FLOAT, static_cast<int>(3 * sizeof(GLfloat)), 3,
								 static_cast<int>(8 * sizeof(GLfloat)));

	program_->enableAttributeArray(2);
	program_->setAttributeBuffer(2, GL_FLOAT, static_cast<int>(6 * sizeof(GLfloat)), 2,
								 static_cast<int>(8 * sizeof(GLfloat)));

	mvpUniform_ = program_->uniformLocation("mvp");

	// Release all
	program_->release();

	vao_.release();

	ibo_.release();
	vbo_.release();

	// Еnable depth test and face culling
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Clear all FBO buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Create camera
	camera_ = Camera(800, 800, {0, 0, -2});
}

void Window::onRender()
{
	const auto guard = captureMetrics();

	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind VAO and shader program
	program_->bind();
	vao_.bind();

	const auto fov = 60.0f;
	const auto zNear = 0.1f;
	const auto zFar = 100.0f;
	program_->setUniformValue(mvpUniform_, camera_.update(fov, zNear, zFar, totalFrameCount_));

	// Activate texture unit and bind texture
	glActiveTexture(GL_TEXTURE0);
	texture_->bind();

	// Draw
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, nullptr);

	// Release VAO and shader program
	texture_->release();
	vao_.release();
	program_->release();

	++frameCount_;
	++totalFrameCount_;

	// Request redraw if animated
	if (animated_)
	{
		update();
	}
}

void Window::onResize(const size_t width, const size_t height)
{
	// Configure viewport
	glViewport(0, 0, static_cast<GLint>(width), static_cast<GLint>(height));

	// Update camera
	camera_.resize(width, height);
}

Window::PerfomanceMetricsGuard::PerfomanceMetricsGuard(std::function<void()> callback)
	: callback_{ std::move(callback) }
{
}

void Window::keyPressEvent(QKeyEvent * e)
{
	camera_.input(e, true);
}

Window::PerfomanceMetricsGuard::~PerfomanceMetricsGuard()
{
	if (callback_)
	{
		callback_();
	}
}

auto Window::captureMetrics() -> PerfomanceMetricsGuard
{
	return PerfomanceMetricsGuard{
		[&] {
			if (timer_.elapsed() >= 1000)
			{
				const auto elapsedSeconds = static_cast<float>(timer_.restart()) / 1000.0f;
				ui_.fps = static_cast<size_t>(std::round(frameCount_ / elapsedSeconds));
				frameCount_ = 0;
				emit updateUI();
			}
		}
	};
}
