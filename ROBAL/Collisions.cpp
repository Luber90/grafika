#include "Collisions.h"
#include <cmath>

void FloorCollision::collAct(Camera* c) {
	glm::vec3 pos = c->getPos();
	if (c->getMode() == 1) {
		if (pos.y < 0.75) {
			c->setPos(glm::vec3(pos.x, 1.5, pos.z));
			c->setOnGround(true);
			c->zeroForce();
		}
	}
	else {
		if (pos.y < 0.5) {
			c->setPos(glm::vec3(pos.x, 0.5, pos.z));
			c->setOnGround(true);
			c->zeroForce();
		}
	}
}

bool RobalCollision::collAct(Camera* c, glm::vec3 pos) {
	glm::vec3 tmp = c->getPos() + c->getKier();
	return (pow(tmp.x - pos.x, 2) + pow(tmp.z - pos.z, 2) <= pow(3, 2));
}
