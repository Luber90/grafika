#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"


float dist(glm::vec3 a, glm::vec3 b);

class Collision {
protected:
	glm::vec3 leftdown, rightup;
public:
	Collision(glm::vec3 a, glm::vec3 b) {
		leftdown = a;
		rightup = b;
	}
	virtual void collAct(Camera* c) = 0;
};

class FloorCollision : public Collision {
public:
	FloorCollision(glm::vec3 a, glm::vec3 b) : Collision(a, b) {

	}
	void collAct(Camera* c);
};

class RobalCollision{
public:
	bool collAct(Camera* c, glm::vec3 pos);
};

class BulletCollision { //kalsa relikt pozdro w chuj
public:
	bool collAct();
};

class EnemyCollision {
public:
	glm::vec3 pos;
	EnemyCollision(glm::vec3 p) {
		pos = p;
	}
	bool collAct(glm::vec3 po) {
		//glm::vec3 I = 0.5f*(rightup - leftdown);
		//glm::vec3 V = pos - I;
		//glm::vec3 po = pos + glm::vec3(0, 0.6, 0);
		//if ((pos.x >= leftdown.x && pos.x <= rightup.x) && (pos.y >= leftdown.y && pos.y <= rightup.y) && (pos.z >= leftdown.z && pos.z <= rightup.z)) return true;
		//else return false;
		return dist(pos, po) < 2;
	}
};

class ObstacleCollision {
public:
	glm::vec3 leftdown, rightup, sr;
	ObstacleCollision(glm::vec3 a, glm::vec3 b, glm::vec3 s) {
		leftdown = a;
		rightup = b;
		sr = s;
	}
	glm::vec3 collAct(glm::vec3 pos) {
		if (dist(pos, sr) < 4) {
			return pos + glm::normalize(pos + glm::vec3(0, -0.5, 0) - sr) * (4 - dist(pos, sr));
		}
		else return pos;

	}
};

