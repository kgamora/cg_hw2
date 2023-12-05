#ifndef CAMERA_H
#define CAMERA_H
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QMouseEvent>

class Camera
{
public:
	QVector3D position;
	QVector3D orientation{0.0f, 0.0f, -1.0f};
	QVector3D up{0.0f, 1.0f, 0.0f};

	bool firstClick = true;

	float aspect = 1.0f;

	float speed = 0.1f;
	float sensitivity = 100.0f;

	QMatrix4x4 model;
	QMatrix4x4 view;
	QMatrix4x4 projection;

	Camera() = default;
	Camera(size_t width, size_t height, QVector3D position);

	QMatrix4x4 update(float fovd, float near, float far, size_t totalFrameCount_);
	void input(QKeyEvent * event, bool release);
	void input(QMouseEvent* event);
	void resize(size_t width, size_t height);
};

#endif // CAMERA_H
