#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaderprogram.h"
#include "Collisions.h"

//klasa modelu, uzywa jej robal, robal mial byc tutaj ale sie bugowalo jak camera.h zawierala model.h i odwrotnie i naraz chcialy z siebie obiekty to robal.h dziedziczy stad

class Model {
protected:
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec4> normals;
	std::vector<glm::vec2> uvs;
	glm::vec3 pos;
public: 
	Model(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv) {
		vertices = vert;
		normals = norm;
		uvs = uv;
		pos = glm::vec3(0, 0, 0);
	}
	std::vector<glm::vec4> getVert();
	std::vector<glm::vec4> getNorm();
	std::vector<glm::vec2> getUv();
	glm::vec3 getPos();
	void SetPos(glm::vec3 v);
};

//to b�d� segmenty robala, �eby mo�na je by�o ��twiej animowa�
class Segment : public Model {
private:


public:
	Segment(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv) : Model(vert, norm, uv) {

	}
	glm::mat4 draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, glm::vec3 pos, float ang);//noramlniue rysuje segment i zwraca macierz przekszta�ce�
	void draw2(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, glm::mat4 M, glm::vec3 przes, float ang);//rysuje zegment w oparciu o poprzedni narysowany, czly macierz M, wi�c ��twiej si� rysuje bez transformacji pojebanych jakich�
};

class Floor : public Model {
private:
	FloorCollision* coll;
public:
	Floor(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv, FloorCollision* c) : Model(vert, norm, uv) {
		coll = c;
	}
	~Floor() {
		delete coll;
	}
	void draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V);
	void colli(Camera* c);
};
