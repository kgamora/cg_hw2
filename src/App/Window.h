#pragma once

#include "camera.h"
#include <Base/GLWidget.hpp>

#include <QElapsedTimer>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>

#include <tinygltf/tiny_gltf.h>

#include <functional>
#include <memory>

class Window final : public fgl::GLWidget
{
	Q_OBJECT
public:
	Window() noexcept;
	~Window() override;

public: // fgl::GLWidget
	void onInit() override;
	void onRender() override;
	void onResize(size_t width, size_t height) override;

private:
	class PerfomanceMetricsGuard final
	{
	public:
		explicit PerfomanceMetricsGuard(std::function<void()> callback);
		~PerfomanceMetricsGuard();

		PerfomanceMetricsGuard(const PerfomanceMetricsGuard &) = delete;
		PerfomanceMetricsGuard(PerfomanceMetricsGuard &&) = delete;

		PerfomanceMetricsGuard & operator=(const PerfomanceMetricsGuard &) = delete;
		PerfomanceMetricsGuard & operator=(PerfomanceMetricsGuard &&) = delete;

	private:
		std::function<void()> callback_;
	};

private:
	[[nodiscard]] PerfomanceMetricsGuard captureMetrics();

signals:
	void updateUI();

private:
	Camera camera_;

	GLint mvpUniform_ = -1;
	GLint mUniform_ = -1;
	GLint sunPositionUniform_ = -1;
	GLint sunColorUniform_ = -1;

	QOpenGLBuffer vbo_{QOpenGLBuffer::Type::VertexBuffer};
	QOpenGLBuffer ibo_{QOpenGLBuffer::Type::IndexBuffer};
	QOpenGLVertexArrayObject vao_;

	std::unique_ptr<QOpenGLTexture> texture_;
	std::unique_ptr<QOpenGLShaderProgram> program_;

	tinygltf::Model model_;
	std::pair<GLuint, std::map<int, GLuint>> vaoAndEbos_;

	QElapsedTimer timer_;
	size_t frameCount_ = 0;
	size_t totalFrameCount_ = 0;

	struct {
		size_t fps = 0;
	} ui_;

	bool animated_ = true;

protected:
	void keyPressEvent(QKeyEvent * e) override;
	void mouseMoveEvent(QMouseEvent* e) override;
};
