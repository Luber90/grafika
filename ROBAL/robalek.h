#pragma once
#include "Model.h"
#include "camera.h"
#include "Collisions.h"
#include <glm/gtc/type_ptr.hpp>


//ta klasa przechoiwuje kilka segemntów i odpowiednio je tam u¿ywa
class Robal{
private:
	glm::vec3 kier;
	Camera* kamera;
	int kameraMode = 0;
	Segment* segment1, * segment2, * segment3;
	glm::vec3 pos;
	float animeang;
	RobalCollision* colli;
	
public:
	Robal(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv, Camera* kam, glm::vec3 posi, RobalCollision* rr, std::vector<glm::vec4> vert2, std::vector<glm::vec4> norm2, std::vector<glm::vec2> uv2);  //konstruktor
	~Robal() {
		delete colli;
		delete segment1;
		delete segment2;
		delete segment3;
	}
	void draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex, glm::vec3 lp1, glm::vec3 lp2);  //dopiero kiedy rysowanie jest tutaj to dziala
	void changeMode();
	glm::vec3 getPos();
	void SetPos(glm::vec3 v);
	glm::vec3* PosPtr() { return &pos; }
	void setAnimeAng(float a);
	bool coll();
};