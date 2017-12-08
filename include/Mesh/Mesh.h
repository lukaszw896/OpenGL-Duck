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
#include <vector>

enum MeshType
{
    particleBilboard,
};
class Mesh {

public:
    MeshType meshType;
    GLfloat xDirection = 0.0f;
    GLfloat yDirection = 0.0f;
    GLfloat zDirection = 0.0f;
    GLfloat speed = 0.0f;
    GLfloat aproxDistance = 0.0f;
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

    void move(GLfloat xMovement, GLfloat yMovement, GLfloat zMovement);

    void moveToOrigin();

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

class SolidSphere
{
private:
    std::vector<GLfloat> vertices;

public:
    int numOfVertices;
    SolidSphere(float radius, unsigned int rings, unsigned int sectors)
    {
        float const R = 1./(float)(rings-1);
        float const S = 1./(float)(sectors-1);
        int r, s;
        numOfVertices = rings * sectors;
        vertices.resize(rings * sectors * 3);

        std::vector<GLfloat>::iterator v = vertices.begin();
        for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
                float const y = sin( -M_PI_2 + M_PI * r * R );
                float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
                float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );
                *v++ = x * radius;
                *v++ = y * radius;
                *v++ = z * radius;

            }
    }
    GLfloat* GetVertices(){
        return &vertices[0];
    }
};


#endif //DUCK_MESH_H
