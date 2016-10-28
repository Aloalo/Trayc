/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Core/CameraHandler.h>

namespace engine
{
    using namespace glm;

    CameraHandler::CameraHandler(const Camera &cam)
        : mCamera(cam)
    {
    }

    CameraHandler::~CameraHandler(void)
    {
    }

    const vec3& CameraHandler::GetPosition() const
    {
        return mCamera.mPosition;
    }

    const Camera& CameraHandler::GetCamera() const
    {
        return mCamera;
    }

    void CameraHandler::SetAspectRatio(float aspect)
    {
        mCamera.mAspectRatio = aspect;
    }

}
