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
#include "myTeapot.h"
#include "snake.h"
#include "shaderprogram.h"
#include "camera.h"
#include "robalek.h"

using namespace std;

Camera* kamera = new Camera(glm::vec3(0, 0, 0));

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

double lastx = 1337, lasty = 1337;
float speed_x = 0;
float speed_y = 0;
float speed_z = 0;
float speed_m = 0;
float aspectRatio = 1;

Robal* robal;
float * podloga_color;
ShaderProgram* sp;



glm::vec3 cubePos(0, 0, 3);

float* vertices = myTeapotVertices;
float* normals = myTeapotVertexNormals;
float* texCoords = myTeapotTexCoords;
float* colors = myTeapotColors;
int vertexCount = myTeapotVertexCount;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_A) speed_x = PI / 2;
        if (key == GLFW_KEY_D) speed_x = -PI / 2;
        if (key == GLFW_KEY_W) speed_y = PI / 2;
        if (key == GLFW_KEY_S) speed_y = -PI / 2;
        if (key == GLFW_KEY_SPACE) speed_z = PI / 2;
        if (key == GLFW_KEY_LEFT_SHIFT) speed_z = -PI / 2;
        if (key == GLFW_KEY_Q) speed_m = PI / 2;
        if (key == GLFW_KEY_E) speed_m = -PI / 2;
        if (key == GLFW_KEY_F) {
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
    if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_A) speed_x = 0;
        if (key == GLFW_KEY_D) speed_x = 0;
        if (key == GLFW_KEY_W) speed_y = 0;
        if (key == GLFW_KEY_S) speed_y = 0;
        if (key == GLFW_KEY_SPACE) speed_z = 0;
        if (key == GLFW_KEY_LEFT_SHIFT) speed_z = 0;
        if (key == GLFW_KEY_Q) speed_m = 0;
        if (key == GLFW_KEY_E) speed_m = 0;
    }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (lastx == 1337 && lasty == 1337) {
        kamera->rotateKier(0, 0);
        lastx = xpos;
        lasty = ypos;
        return;
    }
    float xoffset = lastx - xpos;
    float yoffset = lasty - ypos;
    lastx = xpos;
    lasty = ypos;

    const float sensitivity = 0.15f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    kamera->rotateKier(glm::radians(xoffset), glm::radians(yoffset));
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
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
	sp = new ShaderProgram("v_simplest.glsl", NULL, "f_simplest.glsl");
    std::vector< glm::vec4 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec4 > temp_normals;
    loadOBJ("cube.obj", temp_vertices, temp_uvs, temp_normals);
    cout << "Załadowano" << endl;
    robal = new Robal(temp_vertices, temp_normals, temp_uvs, podloga_color, kamera);
    robal->SetPos(glm::vec3(0, 0, 7));
    kamera->setRob(robal);
    cout << "Stworzono" << endl;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void freeOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************

	delete sp;
    delete kamera;
}


void drawScene(GLFWwindow* window) {
    //************Tutaj umieszczaj kod rysujący obraz******************l
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    glm::mat4 V = glm::lookAt(
    kamera->getPos(),
    kamera->getPos() + kamera->getKier(),
    glm::vec3(0.0, 1.0, 0.0)); //Wylicz macierz widoku
    glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, aspectRatio, 0.01f, 50.0f); //Wylicz macierz rzutowania

    glm::mat4 M = glm::mat4(1.0f);
    M = glm::translate(M, cubePos);


    sp->use();//Aktywacja programu cieniującego
    //Przeslij parametry programu cieniującego do karty graficznej
    glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
    glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
    glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

    glEnableVertexAttribArray(sp->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
    glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, vertices); //Wskaż tablicę z danymi dla atrybutu vertex
    glEnableVertexAttribArray(sp->a("normal"));
    glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals);
    //glEnableVertexAttribArray(sp->a("color"));
    glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colors);


    glDrawArrays(GL_TRIANGLES, 0, vertexCount); //Narysuj obiekt

    //ladowanie kostki
    
    robal->draw(sp, P, V);

    glDisableVertexAttribArray(sp->a("normal"));
    glDisableVertexAttribArray(sp->a("vertex"));
    glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni

}

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
        kamera->rotateKier(speed_x * glfwGetTime(), 0);
        kamera->setPos(kamera->getPos() + kamera->getKier() * speed_y * (float) glfwGetTime() + kamera->getPrawo() * speed_m * (float) glfwGetTime() + glm::vec3(0, speed_z * glfwGetTime(), 0));
        glfwSetTime(0); //Zeruj timer
        drawScene(window); //Wykonaj procedurę rysującą
        glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
    }

    freeOpenGLProgram(window);

    glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
    glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
    exit(EXIT_SUCCESS);
    
}

