#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaderprogram.h"
#include "Collisions.h"
#include "camera.h"

//klasa modelu, uzywa jej robal, robal mial byc tutaj ale sie bugowalo jak camera.h zawierala model.h i odwrotnie i naraz chcialy z siebie obiekty to robal.h dziedziczy stad

class Model {
protected:
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec4> normals;
	std::vector<glm::vec2> uvs; // wspolrzedne teksturowania ??chyba
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

class Sky : public Model {
public:
	Sky(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv) :Model(vert, norm, uv) {}
	void draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex);
};

//to bêd¹ segmenty robala, ¿eby mo¿na je by³o ³¹twiej animowaæ
class Segment : public Model {
private:


public:
	Segment(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv) : Model(vert, norm, uv) {

	}
	glm::mat4 draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, glm::vec3 pos, float ang, GLuint tex, glm::vec3 lp1, glm::vec3 lp2);//noramlniue rysuje segment i zwraca macierz przekszta³ceñ
	void draw2(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, glm::mat4 M, glm::vec3 przes, float ang, GLuint tex, glm::vec3 lp1, glm::vec3 lp2);//rysuje zegment w oparciu o poprzedni narysowany, czly macierz M, wiêc ³¹twiej siê rysuje bez transformacji pojebanych jakichœ
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
	void draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex, glm::vec3 lp1, glm::vec3 lp2);
	void colli(Camera* c);
};

class Bullet : public Model {
private:
	glm::vec3 kier;
public:
	Bullet(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv, glm::vec3 k, glm::vec3 p) : Model(vert, norm, uv) {
		kier = k;
		pos = p;
	}
	void move(float t);
	void draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex, glm::vec4 lp1, glm::vec4 lp2);
	bool floorColl();

};

class BulletVec {
private:
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec4> normals;
	std::vector<glm::vec2> uvs;
	std::vector<Bullet *> vector;
public:
	void add(glm::vec3 pos, glm::vec3 kier);
	void set(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv);
	int size();
	Bullet* operator[] (int i);
	void draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex, glm::vec3 lp1, glm::vec3 lp2);
	void erase(int i);
};

class Enemy : public Model {
private:
	EnemyCollision* coll;
	std::vector<glm::vec4> vertices2;
	std::vector<glm::vec4> normals2;
	std::vector<glm::vec2> uvs2; 
public:
	Enemy(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv, glm::vec3 p, std::vector<glm::vec4> vert2, std::vector<glm::vec4> norm2, std::vector<glm::vec2> uv2) : Model(vert, norm, uv){
		pos = p;
		coll = new EnemyCollision(pos);
		vertices2 = vert2;
		normals2 = norm2;
		uvs2 = uv2;
	}
	bool colli(glm::vec3 p) {
		return coll->collAct(p);
	}
	void draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex, GLuint tex2, glm::vec3 cmpos, glm::vec4 lp1, glm::vec4 lp2);
};

class EnemyVector {
private:
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec4> normals;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec4> vertices2;
	std::vector<glm::vec4> normals2;
	std::vector<glm::vec2> uvs2;
	std::vector<Enemy*> vector;
public:
	void add(glm::vec3 pos);
	void set(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv);
	void set2(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv);
	int size();
	Enemy* operator[] (int i);
	void draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex, GLuint tex2, glm::vec3 cmpos, glm::vec3 lp1, glm::vec3 lp2);
	void coll(glm::vec3 p) {
		for (int i = 0; i < vector.size(); i++) {
			if (vector[i]->colli(p)) {
				std::vector<Enemy*> tmpVector;
				for (int j = 0; j < vector.size(); j++){
					if (!(j == i)) {
						tmpVector.push_back(vector[j]);
					}
					
				}
				vector = tmpVector;
			}
		}
	}
};

class Obstacle : public Model { // "przeszkody" docelowo wydmy czy cos w tym rodzaju
private:
	ObstacleCollision* coll;
	float angle;
public:
	Obstacle(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv, glm::vec3 p, float ang) : Model(vert, norm, uv) {
		pos = p;
		coll = new ObstacleCollision(pos + glm::vec3(-0.5, -0.5, -0.5), pos + glm::vec3(0.5, 0.5, 0.5), pos);
		angle = ang;
	}
	void draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex, glm::vec4 lp1, glm::vec4 lp2);
	glm::vec3 colli(glm::vec3 p) {
		return coll->collAct(p);
		
	}
};

class ObstacleVector {
private:
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec4> normals;
	std::vector<glm::vec2> uvs;
	std::vector<Obstacle*> vector;
public:
	void add(glm::vec3 pos, float ang);
	void set(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv);
	int size();
	Obstacle* operator[] (int i);
	void draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex, glm::vec3 lp1, glm::vec3 lp2);
	void coll(Camera* cam) {
		for (int i = 0; i < vector.size(); i++) {
			cam->setPos(vector[i]->colli(cam->getPos()));
		}
	}

};

class Lamp : public Model {
public:
	Lamp(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv, glm::vec3 p) : Model(vert, norm, uv) {
		pos = p;
	}
	void draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex);
};






