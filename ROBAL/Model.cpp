#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Model.h"



std::vector<glm::vec4> Model::getVert() {
	return vertices;
}
std::vector<glm::vec4> Model::getNorm() {
	return normals;
}
std::vector<glm::vec2> Model::getUv() {
	return uvs;
}
glm::vec3 Model::getPos() {
	return pos;
}

void Model::SetPos(glm::vec3 v) {
	pos = v;
}

glm::mat4 Segment::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, glm::vec3 pos, float ang) {
	sp->use();
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, pos);
	M = glm::rotate(M, ang, glm::vec3(0, 1, 0));
	//M = glm::scale(M, glm::vec3(0.25, 0.25, 0.25));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices[0]);
	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals[0]);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	return M;
}
void Segment::draw2(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, glm::mat4 M, glm::vec3 przes, float ang) {
	sp->use();
	M = glm::translate(M, przes);
	M = glm::rotate(M, ang, glm::vec3(0, 1, 0));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices[0]);
	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals[0]);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void Floor::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex) {
	sp->use();
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::scale(M, glm::vec3(100, 1, 100));

	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(sp->a("texCoord0"));
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, &uvs[0]);

	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices[0]);
	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals[0]);


	glUniform1i(sp->u("textureMap0"), 0);

	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, tex);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisableVertexAttribArray(sp->a("texCoord0"));
}

void Floor::colli(Camera* c) {
	coll->collAct(c);
}

void Bullet::move(float t) {
	pos += 10 * t * kier;
}

void Bullet::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V) {
	sp->use();
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, pos);
	M = glm::scale(M, glm::vec3(0.01, 0.01, 0.01));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices[0]);
	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals[0]);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void BulletVec::add(glm::vec3 pos, glm::vec3 kier) {
	vector.push_back(new Bullet(vertices, normals, uvs, kier, pos));
}

void BulletVec::set(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv) {
	vertices = vert;
	normals = norm;
	uvs = uv;
}

int BulletVec::size() {
	return vector.size();
}

Bullet* BulletVec::operator[] (int i) {
	return vector[i];
}

void BulletVec::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V) {
	for (int i = 0; i < vector.size(); i++) {
		vector[i]->draw(sp, P, V);
	}
}

void Enemy::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex) {
	sp->use();
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::scale(M, glm::vec3(100, 1, 100));

	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(sp->a("texCoord0"));
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, &uvs[0]);

	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices[0]);

	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals[0]);


	glUniform1i(sp->u("textureMap0"), 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisableVertexAttribArray(sp->a("texCoord0"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("color"));
}
void EnemyVector::add(glm::vec3 pos) {
	vector.push_back(new Enemy(vertices, normals, uvs, pos));
}

int EnemyVector::size() {
	return vector.size();
}

Enemy* EnemyVector::operator[] (int i) {
	return vector[i];
}

void EnemyVector::set(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv) {
	vertices = vert;
	normals = norm;
	uvs = uv;
}

void EnemyVector::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex) {
	for (int i = 0; i < vector.size(); i++) {
		vector[i]->draw(sp, P, V, tex);
	}
}

void Obstacle::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V) {
	sp->use();
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, pos);
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices[0]);
	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals[0]);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void ObstacleVector::add(glm::vec3 pos) {
	vector.push_back(new Obstacle(vertices, normals, uvs, pos));
}

int ObstacleVector::size() {
	return vector.size();
}

Obstacle* ObstacleVector::operator[] (int i) {
	return vector[i];
}

void ObstacleVector::set(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv) {
	vertices = vert;
	normals = norm;
	uvs = uv;
}

void ObstacleVector::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V) {
	for (int i = 0; i < vector.size(); i++) {
		vector[i]->draw(sp, P, V);
	}
}