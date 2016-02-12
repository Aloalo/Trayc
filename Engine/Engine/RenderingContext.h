/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_RENDERING_CONTEXT_H
#define EN_RENDERING_CONTEXT_H

#include <glm/glm.hpp>

namespace engine
{
    struct Light;
    class Camera;

    struct RenderingContext
    {
        //View matrix
        glm::mat4 mV;
        //Projection matrix
        glm::mat4 mP;
        //Projection * View
        glm::mat4 mVP;

        //normal matrix = transpose(inverse(mat3(modelView)));

        const Light *mLight;
        const Camera *mCamera;
    };
}

#endif
