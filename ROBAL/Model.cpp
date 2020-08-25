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

void Floor::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V) {
	sp->use();
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::scale(M, glm::vec3(100, 1, 100));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices[0]);
	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals[0]);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void Floor::colli(Camera* c) {
	coll->collAct(c);
}