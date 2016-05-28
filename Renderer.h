//
// Created by lukas on 5/28/2016.
//

#ifndef DUCK_RENDERER_H
#define DUCK_RENDERER_H

#include <SOIL.h>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#include "shaderloader.h"
#include "Camera.h"
// GLFW
#include <glfw3.h>

#include <Mesh/Mesh.h>
#include <Mesh/MeshLoader.h>
#include <c++/vector>

using namespace std;

class Renderer {
public:
    static Renderer& getInstance();
    void render();

private:
    Renderer();
    GLFWwindow* window;
    Camera& camera = Camera::getInstance();
    bool keys[1024];
    GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
    GLfloat lastFrame = 0.0f;  	// Time of last frame
    GLfloat lastX = 400, lastY = 300;
    vector<Mesh*> meshVector;

    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        getInstance().my_scroll_callback(window,xoffset,yoffset);
    }

    void my_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        getInstance().my_key_callback(window,key,scancode,action,mode);
    }

    void my_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);


    static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        getInstance().my_mouse_callback(window,xpos,ypos);
    }

    bool firstMouse = true;
    void my_mouse_callback(GLFWwindow* window, double xpos, double ypos);

    void loadTexture(GLuint* texture, const char* path);

    void do_movement();

};


#endif //DUCK_RENDERER_H
