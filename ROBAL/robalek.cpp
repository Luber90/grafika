#include "robalek.h"

const float PI = 3.141592653589793f;

Robal::Robal(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv, Camera* kam, glm::vec3 posi, RobalCollision* rr, std::vector<glm::vec4> vert2, std::vector<glm::vec4> norm2, std::vector<glm::vec2> uv2){
	kier = glm::vec3(0, 0, 1);
	kamera = kam;
	segment1 = new Segment(vert2, norm2, uv2);
	segment2 = new Segment(vert, norm, uv);
	segment3 = new Segment(vert2, norm2, uv2);
	pos = posi;
	animeang = 0;
	colli = rr;
}

void Robal::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex) { //dopiero kiedy rysowanie jest tutaj to dziala
	if (kameraMode == 1) {
		pos = kamera->getPos() + glm::vec3(0, -1.5, 0);
		glm::mat4 M = segment2->draw(sp, P, V, pos, kamera->getAng(), tex);
		segment1->draw2(sp, P, V, M, glm::vec3(0.4, 0.22, -1.38), PI-animeang, tex);
		segment3->draw2(sp, P, V, M, glm::vec3(0.4, 0.22, 0.1), animeang, tex);
	}	
	else {
		glm::mat4 M = segment2->draw(sp, P, V, pos, 0, tex);
		segment1->draw2(sp, P, V, M, glm::vec3(0.4, 0.22, -1.38), PI, tex);
		segment3->draw2(sp, P, V, M, glm::vec3(0.4, 0.22, 0.1), 0, tex);
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

bool Robal::coll() {
	return colli->collAct(kamera, pos);
}