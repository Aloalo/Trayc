#ifndef RTRT_RTRT_GAMELIKE_H
#define RTRT_RTRT_GAMELIKE_H

#include "RTRTLevel.h"

namespace engine
{
    class RayTraceRenderPass;
}


class RTRTGamelike
{
public:
    RTRTGamelike(engine::RayTraceRenderPass *rtPass);

    void LoadLevel(const std::string &name);

protected:
    engine::RayTraceRenderPass *mRTPass;
    RTRTLevel mLevel;
};

#endif