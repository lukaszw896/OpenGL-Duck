//
// Created by lukas on 5/28/2016.
//

#include "Renderer.h"

Renderer& Renderer::getInstance(){
    static Renderer instance;
    return instance;
}

Renderer::Renderer() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return ;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return ;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);

    glfwSetKeyCallback(window, &key_callback);
    glfwSetCursorPosCallback(window, &mouse_callback);
    glfwSetScrollCallback(window, &scroll_callback);
    //init objects

    MeshLoader& meshLoader = MeshLoader::getMeshLoaderInstance();

    GLuint shaderProgram;
    ShaderLoader::loadProgram(&shaderProgram,"res/shaders/basicVertexShader.txt","res/shaders/basicFragmentShader.txt");
    GLuint texture;
    loadTexture(&texture,"res/textures/container.png");

    Mesh quad = meshLoader.getQuad();
    quad.loadProgram(shaderProgram);
    quad.loadTexture(texture);
    quad.setTranslation(0.f,0.f,-0.5f);
    meshVector.push_back(quad);

    Mesh quad2 = meshLoader.getQuad();
    quad2.loadProgram(shaderProgram);
    quad2.loadTexture(texture);
    quad2.setTranslation(0.f,0.f,0.5);
    quad2.setXRotation(3.14);
    meshVector.push_back(quad2);

    Mesh quad3 = meshLoader.getQuad();
    quad3.loadProgram(shaderProgram);
    quad3.loadTexture(texture);
    quad3.setTranslation(0.5f,0.f,0.f);
    quad3.setYRotation(3.14/2);
    meshVector.push_back(quad3);

    Mesh quad4 = meshLoader.getQuad();
    quad4.loadProgram(shaderProgram);
    quad4.loadTexture(texture);
    quad4.setTranslation(-0.5f,0.f,0.f);
    quad4.setYRotation(-3.14/2);
    meshVector.push_back(quad4);
}

void Renderer::render() {
    //game loop
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //UNIFORM EXAMPLE
        /*// Update the uniform color
        GLfloat timeValue = glfwGetTime();
        GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
        GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);*/
        do_movement();
        camera.updateCameraView();
        for(int i=0;i<meshVector.size();i++){meshVector[i].render();}

        glfwSwapBuffers(window);
    }
    /* glDeleteVertexArrays(1, &VAO);
     glDeleteBuffers(1, &VBO);*/
    glfwTerminate();

    return ;
}

void Renderer::my_scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
        if(camera.fov >= 1.0f && camera.fov <= 45.0f)
            camera.fov -= yoffset;
        if(camera.fov <= 1.0f)
            camera.fov = 1.0f;
        if(camera.fov >= 45.0f)
            camera.fov = 45.0f;
}

void Renderer::my_key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    // When a user presses the escape key, we set the WindowShouldClose property to true,
    // closing the application
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;
}

void Renderer::my_mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if(GLFW_PRESS == glfwGetMouseButton(window,0) && firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    if(GLFW_PRESS == glfwGetMouseButton(window,0)) {
        GLfloat xoffset = xpos - lastX;
        GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates range from bottom to top
        lastX = xpos;
        lastY = ypos;

        GLfloat sensitivity = 0.5f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        camera.yaw += xoffset;
        camera.pitch += yoffset;
        if (camera.pitch > 89.0f)
            camera.pitch = 89.0f;
        if (camera.pitch < -89.0f)
            camera.pitch = -89.0f;
        glm::vec3 front;
        front.x = cos(glm::radians(camera.pitch)) * cos(glm::radians(camera.yaw));
        front.y = sin(glm::radians(camera.pitch));
        front.z = cos(glm::radians(camera.pitch)) * sin(glm::radians(camera.yaw));
        camera.cameraFront = glm::normalize(front);
    } else{
        firstMouse = true;
    }
}

void Renderer::loadTexture(GLuint *texture, const char *path) {
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    // Set the texture wrapping/filtering options (on the currently bound texture object)
    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // Load and generate the texture
    int w, h;
    unsigned char* image = SOIL_load_image(path, &w, &h, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::do_movement() {
    // Camera controls
    GLfloat cameraSpeed = 5.0f * deltaTime;
    if(keys[GLFW_KEY_W])
        camera.cameraPos += cameraSpeed * camera.cameraFront;
    if(keys[GLFW_KEY_S])
        camera.cameraPos -= cameraSpeed * camera.cameraFront;
    if(keys[GLFW_KEY_A])
        camera.cameraPos -= glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;
    if(keys[GLFW_KEY_D])
        camera.cameraPos += glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;
}