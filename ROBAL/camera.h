#pragma once

#include <vector>
#include <glm/glm.hpp>

class Camera {
private:
	glm::vec3 position;
	float angle;
public:
	Camera(glm::vec3 pos, float ang);
	glm::vec3 getPos();
	float getAng();
	void setPos(glm::vec3 v);
	void setAng(float ang);
};
