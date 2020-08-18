#include "camera.h"

Camera::Camera(glm::vec3 pos) {
	position = pos;
	kierunek = glm::vec3(0, 0, 1);
}

void Camera::setPos(glm::vec3 v) {
	position = v;
}

glm::vec3 Camera::getPos() {
	return position;
}

glm::vec3 Camera::getKier() {
	return kierunek;
}

void Camera::rotateKier(float angx, float angy) {
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::rotate(M, angx, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::rotate(M, angy, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::vec4 tmp = M * glm::vec4(kierunek, 0);
	kierunek.x = tmp.x;
	kierunek.y = tmp.y;
	kierunek.z = tmp.z;
}