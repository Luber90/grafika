#include "Collisions.h"

void FloorCollision::collAct(Camera* c) {
	glm::vec3 pos = c->getPos();
	if (pos.y < 0.5) {
		c->setPos(glm::vec3(pos.x, 0.5, pos.z));
		c->setOnGround(true);
		c->zeroForce();
	}
}