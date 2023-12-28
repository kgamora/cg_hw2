#include "camera.h"
#include <iostream>

Camera::Camera(size_t width, size_t height, QVector3D position)
{
	Camera::width = width;
	Camera::height = height;
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

//	model.rotate(angle, {0.f, 1.f, 0.f});
	view.setToIdentity();
//	view.lookAt(position, position + orientation, up);
	view.rotate(rotationX, 1.0, 0.0, 0.0);
	view.rotate(rotationY, 0.0, 1.0, 0.0);
	view.translate(position);

	QVector4D newOrientation4D = QVector4D(orientation, 0.0) * view;
	QVector3D newOrientation = newOrientation4D.toVector3D();

	position += newOrientation * movement;
	movement = {0.0, 0.0, 0.0};

	projection.setToIdentity();
	projection.perspective(fovd, aspect, near, far);

	return projection * view * model;
}

void Camera::input(QKeyEvent * event, bool)
{
	switch( event->key() )
	{
		case Qt::Key_W:
			movement += {0.0, 0.0, -speed};
			break;

		case Qt::Key_A:
			movement += {-speed, 0.0, 0.0};
			break;

		case Qt::Key_S:
			movement += {0.0, 0.0, speed};
			break;

		case Qt::Key_D:
			movement += {speed, 0.0, 0.0};
			break;

		case Qt::Key_Up:
			movement += {0.0, speed, 0.0};
			break;

		case Qt::Key_Down:
			movement += {0.0, -speed, 0.0};
			break;

		default:
			break;
	}
}

void Camera::input(QMouseEvent *mouseEvent)
{
	if (mouseEvent->buttons() & Qt::LeftButton) {

		if (firstClick)
		{
			firstClick = false;
			prevPos = mouseEvent->localPos();
			return;
		}

		auto delta = mouseEvent->localPos() - prevPos;

		float mouseX = delta.x(), mouseY = delta.y();

		mouseX = std::clamp(mouseX, -10.0f, 10.0f);
		mouseY = std::clamp(mouseY, -10.0f, 10.0f);

		rotationX += sensitivity * mouseY;
		rotationY += sensitivity * mouseX;

		prevPos = mouseEvent->localPos();
	} else {
		firstClick = true;
		prevPos = mouseEvent->localPos();
	}
}

void Camera::resize(size_t width, size_t height)
{
	this->aspect = static_cast<float>(width) / static_cast<float>(height);
}
