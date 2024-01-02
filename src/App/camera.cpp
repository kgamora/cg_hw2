#include "camera.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include<glm/gtx/rotate_vector.hpp>

Camera::Camera(size_t width, size_t height, QVector3D position)
{
	Camera::width = width;
	Camera::height = height;
	Camera::aspect = static_cast<float>(width) / static_cast<float>(height);
	Camera::position = position;
}

glm::vec3 toGLMVec3(QVector3D in) {
	return {in.x(), in.y(), in.z()};
}

QVector3D toQVec3(glm::vec3 in) {
	return {in.x, in.y, in.z};
}

std::tuple<QMatrix4x4, QMatrix4x4, QMatrix4x4, QVector3D> Camera::update(float fovd, float near, float far, size_t totalFrameCount_)
{
	// Calculate MVP matrix
	model.setToIdentity();

	glm::vec3 glmOrientation = toGLMVec3(orientation);
	glm::vec3 glmUp = toGLMVec3(up);
	glm::vec3 glmNewOrientation = glm::rotate(glmOrientation, glm::radians(-rotationX), glm::normalize(glm::cross(glmOrientation, glmUp)));
	glmNewOrientation = glm::rotate(glmNewOrientation, glm::radians(-rotationY), glmUp);
	rotationX = 0, rotationY = 0;
	orientation = toQVec3(glmNewOrientation);

	view.setToIdentity();
	view.lookAt(position, position + orientation, up);

	QVector4D newRight4D = QVector4D(right, 1.0) * view;
	QVector3D newRight = newRight4D.toVector3D().normalized();

	// view.translate(position);

	position += movement.z() * orientation;
	position += movement.x() * newRight;
	position += {0.0, movement.y(), 0.0};
	movement = {0.0, 0.0, 0.0};

	projection.setToIdentity();
	projection.perspective(fovd, aspect, near, far);

	return {model, view, projection, orientation};
}

void Camera::input(QKeyEvent * event, bool)
{
	switch( event->key() )
	{
		case Qt::Key_W:
			movement += {0.0, 0.0, speed};
			break;

		case Qt::Key_A:
			movement += {-speed, 0.0, 0.0};
			break;

		case Qt::Key_S:
			movement += {0.0, 0.0, -speed};
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
