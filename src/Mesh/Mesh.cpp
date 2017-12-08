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

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    // Create transformations
    glm::mat4 projection;
    projection = glm::perspective(camera.fov, float(1920)/float(1080), 0.1f, 100.0f);
    glm::mat4 model;

    model = glm::translate(model, glm::vec3(xTranslation,yTranslation,zTranslation));
    model = glm::rotate(model, xRotation, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, yRotation, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, zRotation, glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::scale(model,glm::vec3(scale));

    // Note that we're translating the scene in the reverse direction of where we want to move

    // Get matrix's uniform location and set matrix
     GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
     glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
     GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
     glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.view));
     GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
     glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    if(this->meshType == particleBilboard) {
        GLint perpendicular = glGetUniformLocation(shaderProgram, "perpendicular");
        glUniform1i(perpendicular, 1);
        GLint scaleVS = glGetUniformLocation(shaderProgram, "scale");
        glUniform1f(scaleVS, scale);
    }
    GLint camerafront = glGetUniformLocation(shaderProgram, "camerafront");
    glUniformMatrix4fv(camerafront, 1, GL_FALSE, glm::value_ptr(camera.cameraFront));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES,0,this->numOfVertices);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    ///
    /*// Create transformations
    glm::mat4 projection;
    projection = glm::perspective(camera.fov, 800.f / 480.f, 0.1f, 100.0f);
    glm::mat4 model;
    model = glm::rotate(model, xRotation, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, yRotation, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, zRotation, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 view;
    // Note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(xTranslation,yTranslation,zTranslation));

    // Get matrix's uniform location and set matrix
    /* GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
     glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
     GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
     glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
     GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
     glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));*/
   /* glm::mat4 transform;
    transform = projection*view*model;
    GLint modelLoc = glGetUniformLocation(shaderProgram, "transform");*/
}

void Mesh::renderAsSkyBox() {
    glDepthMask(GL_FALSE);// Remember to turn depth writing off
    glUseProgram(shaderProgram);
    // Create transformations
    glm::mat4 projection;
    projection = glm::perspective(camera.fov, float(1920)/float(1080), 0.1f, 1000.0f);
    glm::mat4 model;
    model = glm::scale(model,glm::vec3(500.0f));

    // Note that we're translating the scene in the reverse direction of where we want to move

    // Get matrix's uniform location and set matrix
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.view));
    GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    // skybox cube
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shaderProgram, "skybox"), 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
}

void Mesh::loadTexture(GLuint texture) {
    this->texture = texture;
}

void Mesh::loadDiffuseMap(GLuint diffuseMap) {
    this->diffuseMap = diffuseMap;
}

void Mesh::loadSpecularMap(GLuint specularMap) {
    this->specularMap = specularMap;
}

void Mesh::loadProgram(GLuint shaderProgram) {
    this->shaderProgram = shaderProgram;
}

void Mesh::setTranslation(GLfloat xTranslation, GLfloat yTranslation, GLfloat zTranslation) {
    this->xTranslation = xTranslation;
    this->yTranslation = yTranslation;
    this->zTranslation = zTranslation;
}

void Mesh::setTranslation(glm::vec3 vec) {
    this->setTranslation(vec.x, vec.y, vec.z);
}

void Mesh::setTranslationWithRespectToOrigin(glm::vec3 vec) {
    this->setTranslation(this->xOrigin + vec.x, this->yOrigin + vec.y, this->zOrigin + vec.z);
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

void Mesh::setScale(GLfloat scale) {
    this->scale = scale;
}

void Mesh::setOrigin(glm::vec3 vec) {
    this->xOrigin = vec[0];
    this->yOrigin = vec[1];
    this->zOrigin = vec[2];
}

void Mesh::move(GLfloat xMovement, GLfloat yMovement, GLfloat zMovement){
    this->xTranslation = xTranslation+xMovement;
    this->yTranslation = yTranslation+yMovement;
    this->zTranslation = zTranslation+zMovement;
}

void  Mesh::moveToOrigin(){
    this->setTranslation(glm::vec3(xOrigin,yOrigin,zOrigin));
}