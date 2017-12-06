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

    void renderAsSkyBox();

    void loadTexture(GLuint texture);

    void loadDiffuseMap(GLuint diffuseMap);

    void loadSpecularMap(GLuint specularMap);

    void loadProgram(GLuint shaderProgram);

    void setTranslation(GLfloat xTranslation, GLfloat yTranslation, GLfloat zTranslation);

    void setTranslationWithRespectToOrigin(glm::vec3);

    void setTranslation(glm::vec3);

    void setRotation(GLfloat, GLfloat, GLfloat);

    void setXRotation(GLfloat);

    void setYRotation(GLfloat);

    void setZRotation(GLfloat);

    void setScale(GLfloat);

    void setOrigin(glm::vec3);

    GLint numOfVertices;
private:

    Camera &camera = Camera::getInstance();
    GLuint VAO;
    GLuint texture;
    GLuint diffuseMap;
    GLuint specularMap;
    GLuint shaderProgram;
    GLfloat xOrigin= 0.0f;
    GLfloat yOrigin = 0.0f;
    GLfloat zOrigin = 0.0f;
    GLfloat xTranslation = 0.0f;
    GLfloat yTranslation = 0.0f;
    GLfloat zTranslation = 0.0f;

    GLfloat xRotation = 0.0f;
    GLfloat yRotation = 0.0f;
    GLfloat zRotation = 0.0f;

    GLfloat scale = 1.f;


};


#endif //DUCK_MESH_H
