//
// Created by lukas on 5/28/2016.
//

#ifndef DUCK_MESHLOADER_H
#define DUCK_MESHLOADER_H


#include <c++/vector>
#include <c++/sstream>
#include "Mesh.h"
//#include "../../external/glfw-3.1.2/deps/linmath.h"


using  namespace std;

struct VAOC
{
    GLuint VAO;
    int vertexCount;
};

class MeshLoader {
public:
    static MeshLoader& getMeshLoaderInstance()
    {
        static MeshLoader instance;
        return instance;
    }
    Mesh* getQuad();
    Mesh* getCube();
    Mesh* getSkyBox();
    Mesh* getDuck(GLuint texture, GLuint diffuseMap, GLuint specularMap, GLuint shader, glm::vec3 translation, GLfloat scale);
    Mesh* getSphere();
    VAOC loadFromAszFile(string path);

private:
    void initQuadBuffers();
    void initCubeBuffers();
    void initSkyBoxBuffers();
    void initSphereBuffers(GLfloat*, int numOfVertices);
    MeshLoader();
    GLfloat* loadVerticesFromfile(string path);
    //Quad
    GLuint quadVAO;
    GLuint cubeVAO;
    GLuint skyBoxVAO;
    VAOC sphereVAOC;
    VAOC duckVAOC;

    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }
    std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, elems);
        return elems;
    }
};


#endif //DUCK_MESHLOADER_H
