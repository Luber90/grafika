﻿
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "snake.h"
#include "shaderprogram.h"
#include "camera.h"
#include "robalek.h"
#include "Model.h""
#include "Collisions.h"
#include "lodepng.h"

using namespace std;
BulletVec bulletVector;
EnemyVector enemyvector;
ObstacleVector obstacleV;
Camera* kamera = new Camera(glm::vec3(0, 1, 0));
Sky* niebo;
glm::vec3 lamp1(1, 2, 1), lamp2(0, 2, 5);

const float PI = 3.141592653589793f;




bool loadOBJ(const char* path, std::vector < glm::vec4 >& out_vertices, std::vector < glm::vec2 >& out_uvs, std::vector < glm::vec4 >& out_normals)
{
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec4 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec4 > temp_normals;

	out_vertices.clear();
	out_uvs.clear();
	out_normals.clear();

#pragma warning (disable : 4996)
	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	while (1) {
		char lineHeader[128];
		//pierwsze slowo linii
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec4 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vertex.a = 1.0f;
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec4 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normal.a = 0.0f;
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec4 vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);

		unsigned int uvsIndex = uvIndices[i];
		glm::vec2 uvs = temp_uvs[uvsIndex - 1];
		out_uvs.push_back(uvs);

		unsigned int normalIndex = normalIndices[i];
		glm::vec4 normal = temp_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}
	return true;
}

double lastx = 999999, lasty = 999999;
float speed_x = 0;
float speed_y = 0;
float speed_z = 0;
float speed_m = 0;
float aspectRatio = 1;

Robal* robal;
Floor* podloga;
float* podloga_color;
ShaderProgram* sp,* spenemy,* sp2,* sprobal,* spniebo;


GLuint tex0; 
GLuint tex1; 
GLuint tex2;
GLuint tex3;
GLuint tex4;
GLuint tex5;

glm::vec3 cubePos(0, 0, 3);





void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_Q) {
			speed_x = PI / 2;
			robal->setAnimeAng(-PI/30);
		}
		if (key == GLFW_KEY_E) {
			speed_x = -PI / 2;
			robal->setAnimeAng(PI / 30);
		}
		if (key == GLFW_KEY_W) speed_y = 15;
		if (key == GLFW_KEY_S) speed_y = -15;
		if (key == GLFW_KEY_SPACE) { //speed_z = PI / 2;
			if (kamera->getOnGround()) {
				kamera->addForce(glm::vec3(0, 30, 0));
				kamera->setOnGround(false);
			}
		}
		//if (key == GLFW_KEY_LEFT_SHIFT) speed_z = -PI / 2;
		if (key == GLFW_KEY_A) speed_m = 15;
		if (key == GLFW_KEY_D) speed_m = -15;
		if (key == GLFW_KEY_F) {     //zmiana na bycie na robalu i na odwrot
			if (robal->coll()) {
				switch (kamera->getMode()) {
				case 0:
					kamera->changeMode();
					robal->changeMode();
					break;
				case 1:
					kamera->changeMode();
					robal->changeMode();
					break;
				}
			}
		}
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_Q) {
			speed_x = 0;
			robal->setAnimeAng(0);
		}
		if (key == GLFW_KEY_E) {
			speed_x = 0;
			robal->setAnimeAng(0);
		}
		if (key == GLFW_KEY_W) speed_y = 0;
		if (key == GLFW_KEY_S) speed_y = 0;
		//if (key == GLFW_KEY_SPACE) speed_z = 0;
		//if (key == GLFW_KEY_LEFT_SHIFT) speed_z = 0;
		if (key == GLFW_KEY_A) speed_m = 0;
		if (key == GLFW_KEY_D) speed_m = 0;
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (lastx == 999999 && lasty == 999999) {
		kamera->rotateKier(0, 0);
		lastx = xpos;
		lasty = ypos;
		return;
	}
	float xoffset = lastx - xpos;
	float yoffset = ypos - lasty;
	lastx = xpos;
	lasty = ypos;

	const float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	if (kamera->getVert() >= PI / 2 && yoffset > 0) {
		yoffset = 0;
	}
	if (kamera->getVert() <= -PI / 2 && yoffset < 0) {
		yoffset = 0;
	}
	if (xoffset != 0) {
		if (xoffset > 0) {
			robal->setAnimeAng(-PI / 30);
		}
		else {
			robal->setAnimeAng(PI / 30);
		}
	}
	else {
		robal->setAnimeAng(0);
	}
	kamera->rotateKier(glm::radians(xoffset), glm::radians(yoffset));
	
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		bulletVector.add(kamera->getPos()+kamera->getKier()*0.5f, kamera->getKier());
		//PlaySound("starwars.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	}
}

GLuint readTexture(const char* filename) {

	GLuint tex;
	glActiveTexture(GL_TEXTURE0);


	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image; //Alokuj wektor do wczytania obrazka
	unsigned width, height; //Zmienne do których wczytamy wymiary obrazka


	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);



	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt


	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	return tex;

}


void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	if (height == 0) return;
	aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
}

void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0, 0.35, 0.6, 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);



	sp = new ShaderProgram("v_obstacle.glsl", NULL, "f_obstacle.glsl");
	spenemy = new ShaderProgram("v_enemy.glsl", NULL, "f_enemy.glsl");
	sp2 = new ShaderProgram("v_simplest2.glsl", NULL, "f_simplest2.glsl");
	sprobal = new ShaderProgram("v_robal.glsl", NULL, "f_robal.glsl");
	spniebo = new ShaderProgram("v_sky.glsl", NULL, "f_sky.glsl");


	std::vector< glm::vec4 > temp_vertices;     //wektory do w czytanych rzeczy
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec4 > temp_normals;
	std::vector< glm::vec4 > temp_vertices2;     
	std::vector< glm::vec2 > temp_uvs2;
	std::vector< glm::vec4 > temp_normals2;


	tex0 = readTexture("pustynia.png");// zaladowanie tekstury
	tex1 = readTexture("metal.png");
	tex2 = readTexture("robal.png");
	tex3 = readTexture("skyy.png");
	tex4 = readTexture("skyyref.png");
	tex5 = readTexture("wood.png");

	loadOBJ("tlow.obj", temp_vertices, temp_uvs, temp_normals);  //ładowanie kostki
	loadOBJ("ogon.obj", temp_vertices2, temp_uvs2, temp_normals2);
	cout << "Załadowano" << endl;
	robal = new Robal(temp_vertices, temp_normals, temp_uvs, kamera, glm::vec3(0, 0, 7), new RobalCollision, temp_vertices2, temp_normals2, temp_uvs2); //tworzenie obiektu robaka z wektorami załądowanego modelu
	
	loadOBJ("square.obj", temp_vertices, temp_uvs, temp_normals);
	podloga = new Floor(temp_vertices, temp_normals, temp_uvs, new FloorCollision(glm::vec3(-100, 0, -100), glm::vec3(100, 0, 100)));
	//robal->SetPos(glm::vec3(0, 0, 7)); //polozenie robala
	loadOBJ("kopula.obj", temp_vertices, temp_uvs, temp_normals);
	niebo = new Sky(temp_vertices, temp_normals, temp_uvs);
	loadOBJ("kula.obj", temp_vertices, temp_uvs, temp_normals);
	bulletVector.set(temp_vertices, temp_normals, temp_uvs);
	
	loadOBJ("nowy.obj", temp_vertices, temp_uvs, temp_normals);
	enemyvector.set(temp_vertices, temp_normals, temp_uvs);
	loadOBJ("enemylufa.obj", temp_vertices, temp_uvs, temp_normals);
	enemyvector.set2(temp_vertices, temp_normals, temp_uvs);
	enemyvector.add(glm::vec3(-10, -0.4, 10));
	enemyvector.add(glm::vec3(1, -0.4, 5));
	enemyvector.add(glm::vec3(1, -0.4, 10));
	enemyvector.add(glm::vec3(1, -0.4, 15));
	//enemyvector.add(glm::vec3(1, -0.4, 20));

	loadOBJ("fence.obj", temp_vertices, temp_uvs, temp_normals);
	obstacleV.set(temp_vertices, temp_normals, temp_uvs);
	obstacleV.add(glm::vec3(10, 1, 10), 0);
	obstacleV.add(glm::vec3(10, 1, 15), PI/2);
	obstacleV.add(glm::vec3(10, 1, 20), PI/2);
	obstacleV.add(glm::vec3(10, 1, 25), 0);
	obstacleV.add(glm::vec3(10, 1, 30), 0);



	kamera->setRob(robal->PosPtr()); //kamera ma wskaźnik na pozycje robala
	cout << "Stworzono" << endl;
	
	//glm::vec3 a;
	//a = robal->getPos();
	//cout << (a.x) << (a.y) << (a.z) << endl;


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void freeOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************

	delete sp;
	delete spenemy;
	delete sp2;
	delete spniebo;
	delete kamera;
}

//funkcja do rysowania nowych obiektów
void drawObject(float* vertices, float* normals, float* colors, int vertexCount, glm::vec3 position, glm::mat4 P, glm::mat4 V, glm::mat4 M, float scale_x, float scale_y, float scale_z, float rotate_y) {
	M = glm::translate(M, position);
	//M = glm::rotate(M, ang, glm::vec3(0, 1, 0));
	M = glm::scale(M, glm::vec3(scale_x, scale_y, scale_z));

	sp->use();

	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));


	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, vertices);

	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals);

	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colors);


	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("color"));
}


void drawScene(GLFWwindow* window) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glm::mat4 V = glm::lookAt(
		kamera->getPos(),
		kamera->getPos()+kamera->getKier(),
		glm::vec3(0.0, 1.0, 0.0)); //Wylicz macierz widoku
	glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, aspectRatio, 0.01f, 250.0f); //Wylicz macierz rzutowania

	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, cubePos);


	//sp->use();//Aktywacja programu cieniującego
	////Przeslij parametry programu cieniującego do karty graficznej
	//glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	//glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	//glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));


	//drawObject(cubeVertices, cubeNormals, cubeColors, vertexCount, glm::vec3(0, -1, 0), P, V, M, 50, 0.1, 50, 0);//narysowałem podłogę jak kox

	//ladowanie kostki
	niebo->draw(spniebo, P, V, tex3);
	enemyvector.draw(spenemy, P, V, tex1, tex4, kamera->getPos());
	robal->draw(sprobal, P, V, tex2); //coś się pierdoli chyba tutaj
	podloga->draw(sp2, P, V, tex0);
	bulletVector.draw(sp2, P, V);
	obstacleV.draw(sp, P, V, tex5);

	//glDisableVertexAttribArray(sp->a("texCoord0"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("color"));

	glDisableVertexAttribArray(spenemy->a("texCoord0"));
	glDisableVertexAttribArray(spenemy->a("normal"));
	glDisableVertexAttribArray(spenemy->a("vertex"));
	glDisableVertexAttribArray(spenemy->a("color"));

	glDisableVertexAttribArray(sp2->a("texCoord0"));
	glDisableVertexAttribArray(sp2->a("normal"));
	glDisableVertexAttribArray(sp2->a("vertex"));
	glDisableVertexAttribArray(sp2->a("color"));
	glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni

}

float deltat = 0;

int main()
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1920, 1080, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące


	glfwSetTime(0); //Zeruj timer
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		
		if (deltat >= 0.016666) {
			kamera->rotateKier(speed_x * glfwGetTime(), 0); //obraca kamere o  kat odpowiedni do speeda z inputu
			kamera->setPos(kamera->getPos() + glm::vec3(kamera->getKier().x, 0, kamera->getKier().z) * speed_y * (float)glfwGetTime() + kamera->getPrawo() * speed_m * (float)glfwGetTime() + glm::vec3(0, speed_z * glfwGetTime(), 0)); //ustawia kamere zgodnie ze speedem
			if (!kamera->getOnGround()) {
				kamera->addForce(glm::vec3(0, -0.25, 0));
			}
			for (int i = 0; i < bulletVector.size(); i++) {
				bulletVector[i]->move(glfwGetTime());
				enemyvector.coll(bulletVector[i]->getPos());
				if (bulletVector[i]->floorColl() || dist(kamera->getPos(), bulletVector[i]->getPos()) > 25) {
					bulletVector.erase(i--);
				}
			}
			kamera->applyForce(glfwGetTime());
			podloga->colli(kamera);
			obstacleV.coll(kamera);
			glfwSetTime(0); //Zeruj timer
			drawScene(window); //Wykonaj procedurę rysującą
			glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
			deltat = 0;
		}
		else {
			deltat += glfwGetTime();
		}
	}
	
	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);

}

