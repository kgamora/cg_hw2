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

public:
	constexpr static float MIN_ANGLE = 10;
	constexpr static float MAX_ANGLE = 100;
	constexpr static float DEFAULT_ANGLE = 70;
	constexpr static float MIN_COORD = -100;
	constexpr static float MAX_COORD = 100;
	constexpr static float DEFAULT_X = 0;
	constexpr static float DEFAULT_Z = 100;
	constexpr static float MIN_PROGRESS = 0;
	constexpr static float MAX_PROGRESS = 100;


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
	void updateFPS(uint);

public slots:
	void setInnerCircleAngle(float);
	void setOuterCircleAngle(float);
	void setLightX(float);
	void setLightZ(float);
	void setMorphingProgress(float);

private:
	Camera camera_;

	GLint mUniform_ = -1, vUniform_ = -1, pUniform_ = -1; // to matrices? struct
	GLint sunPositionUniform_ = -1; // to light struct
	GLint sunColorUniform_ = -1;
	GLint spotlightPositionUniform_ = -1;
	GLint spotlightColorUniform_ = -1;
	GLint spotlightDirectionUniform_ = -1;
	GLint spotlightFirstCosUniform_ = -1, spotlightSecondCosUniform_ = -1; // to vec2
	GLint morphingProgressUniform_ = -1;

	float spotlightFirstAngle_ = DEFAULT_ANGLE, spotlightSecondAngle_ = spotlightFirstAngle_ + DEFAULT_ANGLE;
	QVector3D sunColor_= QVector3D(1.0, 1.0, 1.0), spotlightColor_ = QVector3D(1.0, 1.0, 1.0);
	QVector3D lightPos = {DEFAULT_X, 2, DEFAULT_Z / 100.f};
	GLfloat morphingProgress_ = 0;

	QOpenGLBuffer vbo_{QOpenGLBuffer::Type::VertexBuffer};
	QOpenGLBuffer ibo_{QOpenGLBuffer::Type::IndexBuffer};
	QOpenGLVertexArrayObject vao_;

	std::unique_ptr<QOpenGLTexture> texture_;
	std::unique_ptr<QOpenGLShaderProgram> program_;

	tinygltf::Model model_;
	std::pair<GLuint, std::map<int, GLuint>> vaoAndEbos_;

	QElapsedTimer timer_;
	QElapsedTimer timer2_;
	size_t frameCount_ = 0;
	size_t totalFrameCount_ = 0;

	bool animated_ = true;

protected:
	void keyPressEvent(QKeyEvent * e) override;
	void mouseMoveEvent(QMouseEvent* e) override;
};
