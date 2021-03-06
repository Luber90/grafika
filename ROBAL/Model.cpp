#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "Model.h"
#include <iostream>

const float PI = 3.141592653589793f;

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

void Sky::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex) {
	sp->use();
	glm::mat4 M = glm::mat4(1.0f);

	M = glm::translate(M, pos);
	M = glm::scale(M, glm::vec3(100, 50, 100));
	M = glm::scale(M, glm::vec3(1.1, 1.1, 1.1));
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
}

glm::mat4 Segment::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, glm::vec3 pos, float ang, GLuint tex, glm::vec3 lp1, glm::vec3 lp2) {
	sp->use();
	glm::mat4 M = glm::mat4(1.0f);
	
	M = glm::translate(M, pos);
	M = glm::rotate(M, ang, glm::vec3(0, 1, 0));
	M = glm::scale(M, glm::vec3(0.5, 0.7, 0.5));
	//M = glm::scale(M, glm::vec3(0.25, 0.25, 0.25));
	glUniform4fv(sp->u("lp1"), 1, glm::value_ptr(lp1));
	glUniform4fv(sp->u("lp2"), 1, glm::value_ptr(lp2));
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
	return M;
}
void Segment::draw2(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, glm::mat4 M, glm::vec3 przes, float ang, GLuint tex, glm::vec3 lp1, glm::vec3 lp2) {
	sp->use();
	
	M = glm::translate(M, przes);
	M = glm::rotate(M, ang, glm::vec3(0, 1, 0));
	M = glm::scale(M, glm::vec3(1, 1, 1));
	glUniform4fv(sp->u("lp1"), 1, glm::value_ptr(lp1));
	glUniform4fv(sp->u("lp2"), 1, glm::value_ptr(lp2));
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
}

void Floor::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex, glm::vec3 lp1, glm::vec3 lp2) {
	sp->use();
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::scale(M, glm::vec3(200, 1, 200));

	glUniform4fv(sp->u("lp1"), 1, glm::value_ptr(lp1));
	glUniform4fv(sp->u("lp2"), 1, glm::value_ptr(lp2));
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

void Bullet::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex, glm::vec4 lp1, glm::vec4 lp2) {
	sp->use();
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, pos);
	M = glm::scale(M, glm::vec3(0.01, 0.01, 0.01));
	glUniform4fv(sp->u("lp1"), 1, glm::value_ptr(lp1));
	glUniform4fv(sp->u("lp2"), 1, glm::value_ptr(lp2));
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
}

bool Bullet::floorColl() {
	return pos.y < 0.1;
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

void BulletVec::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex, glm::vec3 lp1, glm::vec3 lp2) {
	for (int i = 0; i < vector.size(); i++) {
		vector[i]->draw(sp, P, V, tex, glm::vec4(lp1, 1), glm::vec4(lp2, 1));
	}
}

void BulletVec::erase(int i) {
	delete vector[i];
	vector.erase(vector.begin() + i);
}

void Enemy::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex, GLuint tex2, glm::vec3 cmpos, glm::vec4 lp1, glm::vec4 lp2) {
	sp->use();
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, pos);
	//M = glm::scale(M, glm::vec3(0.25, 0.25, 0.25));
	//cia�o
	glUniform4fv(sp->u("lp1"),1, glm::value_ptr(lp1));
	glUniform4fv(sp->u("lp2"), 1, glm::value_ptr(lp2));
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
	//lufa
	//std::cout << vangle(glm::vec2(cmpos.x, cmpos.z) - glm::vec2(pos.x, pos.z), glm::vec2(-1, 0)) * 180 / PI << std::endl;
	M = glm::mat4(1.0f);
	M = glm::translate(M,pos+ glm::vec3(0, 0.5, 0));
	M = glm::rotate(M, glm::orientedAngle(glm::normalize(glm::vec2(cmpos.x, cmpos.z) - glm::vec2(pos.x, pos.z)), glm::vec2(-1, 0)), glm::vec3(0, 1, 0));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glEnableVertexAttribArray(sp->a("texCoord0"));
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, &uvs2[0]);
	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices2[0]);
	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals2[0]);
	glUniform1i(sp->u("textureMap0"), 0);
	glUniform1i(sp->u("textureMap1"), 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);
	glDrawArrays(GL_TRIANGLES, 0, vertices2.size());

	glDisableVertexAttribArray(sp->a("texCoord0"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("vertex"));
}
void EnemyVector::add(glm::vec3 pos) {
	vector.push_back(new Enemy(vertices, normals, uvs, pos, vertices2, normals2, uvs2));
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

void EnemyVector::set2(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv) {
	vertices2 = vert;
	normals2 = norm;
	uvs2 = uv;
}

void EnemyVector::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex, GLuint tex2, glm::vec3 cmpos, glm::vec3 lp1, glm::vec3 lp2) {
	for (int i = 0; i < vector.size(); i++) {
		vector[i]->draw(sp, P, V, tex, tex2, cmpos, glm::vec4(lp1, 1), glm::vec4(lp2, 1));
	}
}

void Obstacle::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex, glm::vec4 lp1, glm::vec4 lp2) {
	sp->use();
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, pos+glm::vec3(0, -1.9, 0));
	M = glm::rotate(M, PI / 2 + angle, glm::vec3(0, 1, 0));
	glUniform4fv(sp->u("lp1"), 1, glm::value_ptr(lp1));
	glUniform4fv(sp->u("lp2"), 1, glm::value_ptr(lp2));
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
}

void ObstacleVector::add(glm::vec3 pos, float ang) {
	vector.push_back(new Obstacle(vertices, normals, uvs, pos, ang));
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

void ObstacleVector::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex, glm::vec3 lp1, glm::vec3 lp2) {

	for (int i = 0; i < vector.size(); i++) {
		vector[i]->draw(sp, P, V, tex, glm::vec4(lp1, 1), glm::vec4(lp2, 1));
	}
}

void Lamp::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V, GLuint tex) {
	sp->use();
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, pos);
	M = glm::rotate(M, PI, glm::vec3(1, 0, 0));
	M = glm::scale(M, glm::vec3(0.25, 0.25, 0.25));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glEnableVertexAttribArray(sp->a("TexCoord"));
	glVertexAttribPointer(sp->a("TexCoord"), 2, GL_FLOAT, false, 0, &uvs[0]);
	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices[0]);

	glUniform1i(sp->u("Texture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}