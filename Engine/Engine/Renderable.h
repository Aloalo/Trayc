/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_RENDERABLE_H
#define EN_RENDERABLE_H

#include <glm/glm.hpp>
#include <GL/glew.h>

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


    class Renderable
    {
    public:
        Renderable(void);
        virtual ~Renderable(void);

        virtual void Draw(const RenderingContext &rContext) const = 0;

        bool mIsActive;
    };
}

#endif
