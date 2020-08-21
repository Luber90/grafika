#include "robalek.h"

Robal::Robal(std::vector<glm::vec4> vert, std::vector<glm::vec4> norm, std::vector<glm::vec2> uv, float* color, Camera* kam) : Model(vert, norm, uv, color) { //konstruktor, uzwajacy kontruktora Model
	kier = glm::vec3(0, 0, 1);
	kamera = kam;
}

void Robal::draw(ShaderProgram* sp, glm::mat4 P, glm::mat4 V) { //dopiero kiedy rysowanie jest tutaj to dziala
	sp->use();
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, pos);
	if (kameraMode == 1) {
		pos = kamera->getPos() + glm::vec3(0, -1, 0);
	}
	
	if (kameraMode == 1) {
		M = glm::rotate(M, kamera->getAng(), glm::vec3(0, 1, 0));
	}
	M = glm::scale(M, glm::vec3(0.25, 0.25, 0.25));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices[0]);
	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals[0]);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

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