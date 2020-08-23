#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


//jakies potrzebne rzeczy do kamerki

class Camera {
private:
	glm::vec3 position;
	glm::vec3 kierunek; //kierunek patrzenia
	glm::vec3 prawo;  //kierunek w prawo od kamery
	float ang;
	int mode;
	glm::vec3* robpos;
public:
	Camera(glm::vec3 pos);
	glm::vec3 getPos();
	glm::vec3 getKier();
	glm::vec3 getPrawo();
	void setPos(glm::vec3 v);
	void rotateKier(float angx, float angy);
	float getAng();
	void changeMode();
	int getMode() { return mode; }
	void setRob(glm::vec3* rob) { robpos = rob; }
};
