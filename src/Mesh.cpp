//
// Created by lukas on 5/27/2016.
//

#include "Mesh.h"

Mesh::Mesh(Camera* camera) {

    this->camera = camera;
    GLfloat vertices[] = {
            // Positions          // Colors           // Texture Coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Top Left
    };

    GLuint indices[] = {  // Note that we start from 0!
            0, 1, 3,   // First Triangle
            1, 2, 3    // Second Triangle
    };

    GLfloat texCoords[] = {
            0.0f, 0.0f,  // Lower-left corner
            1.0f, 0.0f,  // Lower-right corner
            0.5f, 1.0f   // Top-center corner
    };

    //generate id for vertex buffered object
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 3. Then set our vertex attributes pointers
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    //Texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT,GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    //4. Unbind the VAO
    glBindVertexArray(0);
}

void Mesh::render(){
    // Draw the triangle
    glUseProgram(shaderProgram);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Create transformations
    glm::mat4 projection;
    projection = glm::perspective(camera->fov, 800.f / 480.f, 0.1f, 100.0f);
    glm::mat4 view;
    // Note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 transform;
    transform = glm::translate(transform, glm::vec3(xTranslation,yTranslation,zTranslation));
    transform = glm::rotate(transform, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    transform = projection*camera->view*transform;

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