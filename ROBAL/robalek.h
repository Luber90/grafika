#pragma once
#include "Model.h"
#include "camera.h"

class Robal : public Model {
private:
	glm::vec3 kier;
	Camera* kamera;
	int kameraMode = 0;
public:
	Robal(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv, float* color, Camera* kam);  //konstruktor
	void draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V);  //dopiero kiedy rysowanie jest tutaj to dziala
	void changeMode();
};