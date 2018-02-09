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
#include <DuckMovement.h>


using namespace std;

class Renderer {
public:
    static Renderer& getInstance();
    void render();
    void moveParticles(GLfloat dt);

private:
    Renderer();
    GLFWwindow* window;
    bool isMSAAEnabled;
    Camera& camera = Camera::getInstance();
    bool keys[1024];
    GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
    GLfloat lastFrame = 0.0f;  	// Time of last frame
    GLfloat lastX = 400, lastY = 300;
    vector<Mesh*> meshVector;
    vector<Mesh*> particleVector;
    glm::vec3 lightPos;
    GLuint lightShader;
    GLuint skyBoxShader;
    GLuint waterShader;
    GLuint glassShader;
    GLuint fishEyeShader;

    GLuint skyBoxTexture;
    GLuint cubeTexture;

    Mesh* duck;
    Mesh* duck2;
    Mesh* skyBox;
    Mesh* mySphere;
    DuckMovement duckMovement;


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
    GLuint loadCubemap(vector<const GLchar*> faces);

    void do_movement();
    void prepareDuckScene();
    void drawDuckScene();
    void prepareFishEyeScene();
    void drawFishEyeScene();

};


#endif //DUCK_RENDERER_H
