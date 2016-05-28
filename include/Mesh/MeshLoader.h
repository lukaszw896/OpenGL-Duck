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
    Mesh getQuad();

private:
    MeshLoader();

    //Quad
    GLuint quadVBO;
    GLuint quadVAO;
    GLuint quadEBO;
};


#endif //DUCK_MESHLOADER_H
