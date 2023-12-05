#include "camera.h"

Camera::Camera(size_t width, size_t height, QVector3D position)
{
	Camera::aspect = static_cast<float>(width) / static_cast<float>(height);
	Camera::position = position;
}

QMatrix4x4 Camera::update(float fovd, float near, float far, size_t totalFrameCount_)
{
	// Calculate MVP matrix
	model.setToIdentity();
	model.translate(0, 0, -2);
	// This is temporary
	float angle = (float)totalFrameCount_ * 0.5f;

	model.rotate(angle, {0.f, 1.f, 0.f});
	view.setToIdentity();

	projection.setToIdentity();
	projection.perspective(fovd, aspect, near, far);

	return projection * view * model;
}

void Camera::input(QKeyEvent *, bool)
{

}

void Camera::input(QMouseEvent *)
{

}

void Camera::resize(size_t width, size_t height)
{
	this->aspect = static_cast<float>(width) / static_cast<float>(height);
}
