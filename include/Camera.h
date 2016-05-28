//
// Created by lukas on 5/27/2016.
//

#ifndef DUCK_CAMERA_H
#define DUCK_CAMERA_H


#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <c++/cstdio>

class Camera {
public:

    static Camera& getInstance()
    {
        static Camera instance;

        return instance;
    }

    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

    GLfloat yaw;
    GLfloat pitch;

    GLfloat fov=45.f;

    glm::mat4 view;

    void updateCameraView()
    {
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

private:
    Camera(){printf("kek");}
};


#endif //DUCK_CAMERA_H
