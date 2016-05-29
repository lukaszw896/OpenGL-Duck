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
     lightPos =  glm::vec3(1.2f, 1.0f, 2.0f);

    MeshLoader& meshLoader = MeshLoader::getMeshLoaderInstance();

    lightShader;
    //ShaderLoader::loadProgram(&shaderProgram,"res/shaders/basicVertexShader.txt","res/shaders/basicFragmentShader.txt");
    GLuint diffuseTexture;
    loadTexture(&diffuseTexture,"res/textures/container2.png");
    GLuint specularTexture;
    loadTexture(&specularTexture,"res/textures/container2_specular.png");
    ShaderLoader::loadProgram(&lightShader,"res/shaders/standardVertexShader.vs","res/shaders/lightColorFragmentShader.fs");


    /* Mesh* quad = meshLoader.getQuad();
     quad->loadProgram(shaderProgram);
     quad->loadTexture(texture);
     quad->setTranslation(0.f,0.f,-0.5f);
     meshVector.push_back(quad);

     Mesh* quad2 = meshLoader.getQuad();
     quad2->loadProgram(shaderProgram);
     quad2->loadTexture(texture);
     quad2->setTranslation(0.f,0.f,0.5f);
     quad2->setXRotation(3.14);
     meshVector.push_back(quad2);

     Mesh* quad3 = meshLoader.getQuad();
     quad3->loadProgram(shaderProgram);
     quad3->loadTexture(texture);
     quad3->setTranslation(0.5f,0.f,0.f);
     quad3->setYRotation(3.14/2);
     meshVector.push_back(quad3);

     Mesh* quad4 = meshLoader.getQuad();
     quad4->loadProgram(shaderProgram);
     quad4->loadTexture(texture);
     quad4->setTranslation(-0.5f,0.f,0.f);
     quad4->setYRotation(-3.14/2);
     meshVector.push_back(quad4);

     Mesh* cube = meshLoader.getCube();
     cube->loadProgram(shaderProgram);
     cube->loadTexture(texture);
     cube->setTranslation(4.f,0,0);
     meshVector.push_back(cube);*/

    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    Mesh* cubes;
    for(int i=0;i<10;i++)
    {
        cubes = meshLoader.getCube();
        cubes->loadProgram(lightShader);
       // cubes->loadTexture(cubeTexture);
        cubes->loadDiffuseMap(diffuseTexture);
        cubes->loadSpecularMap(specularTexture);
        cubes->setTranslation(cubePositions[i].x,cubePositions[i].y,cubePositions[i].z);
        meshVector.push_back(cubes);
    }

    Mesh* duck = meshLoader.loadFromAszFile("res/models/duck.txt");
    duck->loadProgram(lightShader);
    GLuint duckTex;
    loadTexture(&duckTex,"res/textures/ducktex.jpg");
    duck->loadDiffuseMap(duckTex);
    duck->loadSpecularMap(duckTex);
    duck->setTranslation(0.f,1.f,0.f);
    duck->setScale(0.01f);
    meshVector.push_back(duck);

    GLuint lampShader;
    ShaderLoader::loadProgram(&lampShader,"res/shaders/lampVertexShader.vs","res/shaders/lampFragmentShader.fs");

    Mesh* lamp = meshLoader.getCube();
    lamp->loadProgram(lampShader);
    lamp->setTranslation(lightPos);
    lamp->setScale(0.2f);
    meshVector.push_back(lamp);
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


        glUseProgram(lightShader);
        GLint objectColorLoc = glGetUniformLocation(lightShader, "objectColor");
        GLint lightColorLoc  = glGetUniformLocation(lightShader, "lightColor");
        GLint lightPosLoc = glGetUniformLocation(lightShader, "lightPos");
        GLint viewPosLoc = glGetUniformLocation(lightShader, "viewPos");
        glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
        glUniform3f(lightColorLoc,  1.0f, 1.0f, 1.0f);
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(viewPosLoc, camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);

        /*GLint matAmbientLoc  = glGetUniformLocation(lightShader, "material.ambient");
        GLint matDiffuseLoc  = glGetUniformLocation(lightShader, "material.diffuse");*/
        GLint matSpecularLoc = glGetUniformLocation(lightShader, "material.specular");
        GLint matShineLoc    = glGetUniformLocation(lightShader, "material.shininess");

        /*glUniform3f(matAmbientLoc,  1.0f, 0.5f, 0.31f);
        glUniform3f(matDiffuseLoc,  1.0f, 0.5f, 0.31f);*/
        glUniform3f(matSpecularLoc, 0.5f, 0.5f, 0.5f);
        glUniform1f(matShineLoc,    32.0f);

        GLint lightAmbientLoc  = glGetUniformLocation(lightShader, "light.ambient");
        GLint lightDiffuseLoc  = glGetUniformLocation(lightShader, "light.diffuse");
        GLint lightSpecularLoc = glGetUniformLocation(lightShader, "light.specular");

        glm::vec3 lightColor;
       /* lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);*/
        lightColor.x = 1.f;
        lightColor.y = 1.f;
        lightColor.z = 1.f;

        glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // Decrease the influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // Low influence

        glUniform3f(lightAmbientLoc, ambientColor.x, ambientColor.y, ambientColor.z);
        glUniform3f(lightDiffuseLoc, diffuseColor.x, diffuseColor.y, diffuseColor.z);
        glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

        glUniform1i(glGetUniformLocation(lightShader, "material.diffuse"), 0);
        glUniform1i(glGetUniformLocation(lightShader, "material.specular"), 1);
        //UNIFORM EXAMPLE
        /*// Update the uniform color
        GLfloat timeValue = glfwGetTime();
        GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
        GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);*/
        do_movement();
        camera.updateCameraView();
        for(int i=0;i<meshVector.size();i++){meshVector[i]->render();}

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
