#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
	glm::vec3 position;
	glm::vec3 kierunek;
	glm::vec3 prawo;
public:
	Camera(glm::vec3 pos);
	glm::vec3 getPos();
	glm::vec3 getKier();
	glm::vec3 getPrawo();
	void setPos(glm::vec3 v);
	void rotateKier(float angx, float angy);
};
