//
// Created by lukas on 5/28/2016.
//

#ifndef DUCK_MESHLOADER_H
#define DUCK_MESHLOADER_H


#include <c++/vector>
#include <c++/sstream>
#include "Mesh.h"


using  namespace std;

class MeshLoader {
public:
    static MeshLoader& getMeshLoaderInstance()
    {
        static MeshLoader instance;
        return instance;
    }

    void initQuadBuffers();
    void initCubeBuffers();
    Mesh* getQuad();
    Mesh* getCube();
    Mesh* loadFromAszFile(string path);

private:
    MeshLoader();
    GLfloat* loadVerticesFromfile(string path);
    //Quad
    GLuint quadVAO;

    GLuint cubeVAO;

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
