//
// Created by lukas on 5/27/2016.
//

#ifndef DUCK_MESH_H
#define DUCK_MESH_H
#include <SOIL.h>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#include "shaderloader.h"
#include "Camera.h"
// GLFW
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class Mesh {


public:
    Mesh(GLuint VAO);

    void render();
    void loadTexture(GLuint texture);
    void loadProgram(GLuint shaderProgram);
    void setTranslation(GLfloat xTranslation, GLfloat yTranslation, GLfloat zTranslation);
    void setRotation(GLfloat,GLfloat,GLfloat);
    void setXRotation(GLfloat);
    void setYRotation(GLfloat);
    void setZRotation(GLfloat);
    GLint numOfVertices;
private:

    Camera& camera = Camera::getInstance() ;
    GLuint VAO;
    GLuint texture;
    GLuint shaderProgram;

    GLfloat xTranslation = 0.0f;
    GLfloat yTranslation = 0.0f;
    GLfloat zTranslation = 0.0f;

    GLfloat xRotation = 0.0f;
    GLfloat yRotation = 0.0f;
    GLfloat zRotation = 0.0f;




};


#endif //DUCK_MESH_H
