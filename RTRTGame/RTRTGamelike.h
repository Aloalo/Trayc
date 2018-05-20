#ifndef RTRT_RTRT_GAMELIKE_H
#define RTRT_RTRT_GAMELIKE_H

#include "RTRTLevel.h"

namespace engine
{
    class RayTraceRenderPass;
    class Camera;
}


class RTRTGamelike
{
public:
    RTRTGamelike(engine::RayTraceRenderPass *rtPass, const engine::Camera *camera);

    void LoadLevel(const std::string &name);

protected:
    engine::RayTraceRenderPass *mRTPass;
    const engine::Camera *mCamera;
    RTRTLevel mLevel;
};

#endif