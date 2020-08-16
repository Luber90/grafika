#include "camera.h"

Camera::Camera(glm::vec3 pos, float ang) {
	position = pos;
	angle = ang;
}

void Camera::setPos(glm::vec3 v) {
	position = v;
}

void Camera::setAng(float ang) {
	angle = ang;
}

glm::vec3 Camera::getPos() {
	return position;
}

float Camera::getAng() {
	return angle;
}