#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaderprogram.h"

class Model {
protected:
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec4> normals;
	std::vector<glm::vec2> uvs;
	glm::vec3 pos;
	float* colors;
public:
	Model(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv, float* color) {
		vertices = vert;
		normals = norm;
		uvs = uv;
		colors = color;
		pos = glm::vec3(0, 0, 0);
	}
	float* getColors();
	std::vector<glm::vec4> getVert();
	std::vector<glm::vec4> getNorm();
	std::vector<glm::vec2> getUv();
	glm::vec3 getPos();
	void SetPos(glm::vec3 v);
};
