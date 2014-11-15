/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Core/CameraHandler.h>

namespace engine
{
    using namespace glm;

    CameraHandler::CameraHandler(const Camera &cam)
        : cam(cam)
    {
    }

    CameraHandler::~CameraHandler(void)
    {
    }

    const vec3& CameraHandler::GetPosition() const
    {
        return cam.position;
    }
}
