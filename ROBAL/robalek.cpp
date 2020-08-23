#include "robalek.h"

Robal::Robal(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv, Camera* kam, glm::vec3 posi){ 
	kier = glm::vec3(0, 0, 1);
	kamera = kam;
	segment1 = new Segment(vert, norm, uv);
	segment2 = new Segment(vert, norm, uv);
	segment3 = new Segment(vert, norm, uv);
	pos = posi;
	animeang = 0;
}

void Robal::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V) { //dopiero kiedy rysowanie jest tutaj to dziala
	if (kameraMode == 1) {
		pos = kamera->getPos() + glm::vec3(0, -1, 0);
		glm::mat4 M = segment2->draw(sp, P, V, pos, kamera->getAng());
		segment1->draw2(sp, P, V, M, glm::vec3(0, 0, 1), -animeang);
		segment3->draw2(sp, P, V, M, glm::vec3(0, 0, -1), animeang);
	}	
	else {
		glm::mat4 M = segment2->draw(sp, P, V, pos, 0);
		segment1->draw2(sp, P, V, M, glm::vec3(0, 0, 1), 0);
		segment3->draw2(sp, P, V, M, glm::vec3(0, 0, -1), 0);
	}
}

void Robal::changeMode() {
	switch (kameraMode) {
	case 0:
		kameraMode = 1;
		break;
	case 1:
		kameraMode = 0;
		break;
	}
}

glm::vec3 Robal::getPos() {
	return pos;
}
void Robal::SetPos(glm::vec3 v) {
	pos = v;
}

void Robal::setAnimeAng(float a) {
	animeang = a;
}