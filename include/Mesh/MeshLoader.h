//
// Created by lukas on 5/28/2016.
//

#ifndef DUCK_MESHLOADER_H
#define DUCK_MESHLOADER_H


#include "Mesh.h"

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

private:
    MeshLoader();

    //Quad
    GLuint quadVAO;

    GLuint cubeVAO;
};


#endif //DUCK_MESHLOADER_H
