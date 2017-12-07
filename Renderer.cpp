//
// Created by lukas on 5/28/2016.
//

#include "Renderer.h"

Renderer& Renderer::getInstance(){
    static Renderer instance;
    return instance;
}

Renderer::Renderer() {
    this->isMSAAEnabled = true;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    auto monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    //window = glfwCreateWindow(2560, 1440, "My Title", monitor, NULL);
    window = glfwCreateWindow(1920, 1080, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);

    glfwSetKeyCallback(window, &key_callback);
    glfwSetCursorPosCallback(window, &mouse_callback);
    glfwSetScrollCallback(window, &scroll_callback);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_FOG);                   // Enables GL_FOG
    //init objects
    lightPos = glm::vec3(20.5f, 8.0f, -0.0f);

    MeshLoader &meshLoader = MeshLoader::getMeshLoaderInstance();
    duckMovement = DuckMovement();

    lightShader;
    //ShaderLoader::loadProgram(&shaderProgram,"res/shaders/basicVertexShader.txt","res/shaders/basicFragmentShader.txt");
    GLuint diffuseTexture;
    loadTexture(&diffuseTexture, "res/textures/container2.png");
    GLuint specularTexture;
    loadTexture(&specularTexture, "res/textures/container2_specular.png");
    ShaderLoader::loadProgram(&lightShader, "res/shaders/standardVertexShader.vs",
                              "res/shaders/lightColorFragmentShader.fs");

    //Cube map

    vector<const GLchar *> faces;
    faces.push_back("res/textures/sor_sea/sea_rt.jpg");
    faces.push_back("res/textures/sor_sea/sea_lf.jpg");
    faces.push_back("res/textures/sor_sea/sea_up.jpg");
    faces.push_back("res/textures/sor_sea/sea_dn.jpg");
    faces.push_back("res/textures/sor_sea/sea_bk.jpg");
    faces.push_back("res/textures/sor_sea/sea_ft.jpg");
    GLuint cubemapTexture = loadCubemap(faces);

    ShaderLoader::loadProgram(&skyBoxShader, "res/shaders/skyBoxVertexShader.vs",
                              "res/shaders/skyBoxFragmentShader.fs");

    skyBox = meshLoader.getSkyBox();

    skyBox->loadTexture(cubemapTexture);
    skyBox->loadProgram(skyBoxShader);
    skyBox->setScale(10.0f);

    GLuint duckTex;
    loadTexture(&duckTex, "res/textures/ducktex.jpg");
    duck = meshLoader.getDuck(duckTex, duckTex, duckTex, lightShader, vec3{5.0f, 0.0f, 7.0f}, 0.002);
    meshVector.push_back(duck);
    duck2 = meshLoader.getDuck(duckTex, duckTex, duckTex, lightShader, vec3{-5.0f, 0.0f, -12.0f}, 0.002);


    /*GLuint sphereProgram;
    ShaderLoader::loadProgram()*/
    mySphere = meshLoader.getSphere();
    mySphere->loadProgram(lightShader);
    mySphere->loadTexture(duckTex);
    mySphere->loadDiffuseMap(duckTex);
    mySphere->loadSpecularMap(duckTex);
    //mySphere->setTranslation(-1.3f, 1.0f, -1.5f);
    meshVector.push_back(mySphere);

    meshVector.push_back(duck2);
    waterShader;
    ShaderLoader::loadProgram(&waterShader, "res/shaders/waterReflectionVertexShader.vs",
                              "res/shaders/waterReflectionFragmentShader.fs");
    Mesh *quad = meshLoader.getQuad();
    quad->loadProgram(waterShader);
    //quad->loadTexture(texture);
    quad->setTranslation(0.f, 0.f, 0.0f);
    //quad->setXRotation(-3.14/2);
    quad->setRotation(3.14 / 2, 0.0f, 3.14 / 2);
    //quad->setYRotation(-3.14/2);
    quad->setScale(300.f);
    meshVector.push_back(quad);

    GLuint bilboardProgram;
    ShaderLoader::loadProgram(&bilboardProgram,"res/shaders/billboard.vs",
                              "res/shaders/billboard.fs");
    GLuint fireTex;
    loadTexture(&fireTex, "res/textures/alphaFire.png");
    Mesh* bilboard = meshLoader.getQuad();
    bilboard->loadProgram(bilboardProgram);
    bilboard->loadTexture(fireTex);
    bilboard->loadDiffuseMap(fireTex);
    bilboard->loadSpecularMap(fireTex);
    bilboard->setRotation(0.0f,-1.5f,0.0f);
    bilboard->setTranslation(0.0f,2.0f,0.0f);
    meshVector.push_back(bilboard);

    Mesh *cubes;
    cubes = meshLoader.getCube();
    cubes->loadProgram(lightShader);
    // cubes->loadTexture(cubeTexture);
    cubes->loadDiffuseMap(diffuseTexture);
    cubes->loadSpecularMap(specularTexture);
    cubes->setTranslation(0.0f, 0.0f, 0.0f);
    meshVector.push_back(cubes);



    GLuint lampShader;
    ShaderLoader::loadProgram(&lampShader, "res/shaders/lampVertexShader.vs", "res/shaders/lampFragmentShader.fs");

    Mesh *lamp = meshLoader.getCube();
    lamp->loadProgram(lampShader);
    lamp->setTranslation(lightPos);
    lamp->setScale(0.5f);
    meshVector.push_back(lamp);
}

void Renderer::render() {
    //game loop
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glm::vec2 duckPosition = duckMovement.getCoords(deltaTime);
        glm::vec3 duckTranslation = vec3(duckPosition.x,-0.04,duckPosition.y);
        duck->setTranslationWithRespectToOrigin(duckTranslation);
        duck2->setTranslationWithRespectToOrigin(duckTranslation);
        glm::vec2 md = duckMovement.getMovementDirection();
        //CALCULATE ROTATION
        float angle = acos(md.x/sqrt(pow(md.x,2)+pow(md.y,2)));
        duck->setRotation(0.0,angle+3.14,0.0);
        duck2->setRotation(0.0,angle+3.14,0.0);
        glUseProgram(waterShader);
        GLint viewPosLoc = glGetUniformLocation(waterShader, "viewPos");
        glUniform3f(viewPosLoc, camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);
        //FIRST DRAW SKYBOX
        //skyBox->setScale(5.f);


        glUseProgram(lightShader);
        GLint objectColorLoc = glGetUniformLocation(lightShader, "objectColor");
        GLint lightColorLoc  = glGetUniformLocation(lightShader, "lightColor");
        GLint lightPosLoc = glGetUniformLocation(lightShader, "lightPos");
        viewPosLoc = glGetUniformLocation(lightShader, "viewPos");
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
        glUniform3f(matSpecularLoc, 0.9f, 0.9f, 0.9f);
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
        skyBox->renderAsSkyBox();

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
    else if(key == GLFW_KEY_1&& action == GLFW_PRESS){
        if(this->isMSAAEnabled){
            this->isMSAAEnabled = false;
            glDisable(GL_MULTISAMPLE);
            printf("MSAA DISABLED \n");
        }
        else{
            this->isMSAAEnabled = true;
            glEnable(GL_MULTISAMPLE);
            printf("MSAA ENABLED \n");
        }
    }

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
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
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

GLuint Renderer::loadCubemap(vector<const GLchar*> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);

    int width,height;
    unsigned char* image;

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for(GLuint i = 0; i < faces.size(); i++)
    {
        image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
        );
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}