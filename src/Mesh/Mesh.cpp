//
// Created by lukas on 5/27/2016.
//

#include "Mesh/Mesh.h"

Mesh::Mesh(GLuint vao) {
    this->VAO = vao;
}

void Mesh::render(){
    // Draw the triangle
    glUseProgram(shaderProgram);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Create transformations
    glm::mat4 projection;
    projection = glm::perspective(camera.fov, 800.f / 480.f, 0.1f, 100.0f);
    glm::mat4 view;
    // Note that we're translating the scene in the reverse direction of where we want to move
    glm::mat4 transform;
    transform = glm::translate(transform, glm::vec3(xTranslation,yTranslation,zTranslation));
    transform = glm::rotate(transform, xRotation, glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, yRotation, glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, zRotation, glm::vec3(0.0f, 0.0f, 1.0f));
    transform = projection*camera.view*transform;

    // Get matrix's uniform location and set matrix
    GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::loadTexture(GLuint texture) {
    this->texture = texture;
}

void Mesh::loadProgram(GLuint shaderProgram) {
    this->shaderProgram = shaderProgram;
}

void Mesh::setTranslation(GLfloat xTranslation, GLfloat yTranslation, GLfloat zTranslation) {
    this->xTranslation = xTranslation;
    this->yTranslation = yTranslation;
    this->zTranslation = zTranslation;
}

void Mesh::setRotation(GLfloat rotateX, GLfloat rotateY, GLfloat rotateZ) {
    setXRotation(rotateX);
    setYRotation(rotateY);
    setZRotation(rotateZ);
}

void Mesh::setXRotation(GLfloat xRotation) {
    this->xRotation = xRotation;
}

void Mesh::setYRotation(GLfloat yRotation) {
    this->yRotation = yRotation;
}

void Mesh::setZRotation(GLfloat zRotation) {
    this->zRotation = zRotation;
}
