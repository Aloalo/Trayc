#ifndef RTRT_RTRT_GAMELIKE_H
#define RTRT_RTRT_GAMELIKE_H

#include "RTRTLevel.h"
#include "Animation.h"

namespace engine
{
    class RayTraceRenderPass;
    class Camera;
}


class RTRTGamelike
{
public:
    RTRTGamelike(engine::RayTraceRenderPass *rtPass, const engine::Camera *camera);
    virtual ~RTRTGamelike(void);

    void SetLevelFromPass();
    void LoadLevel(const std::string &name);
    void RemoveObject(RTRTObject *object);

protected:
    engine::RayTraceRenderPass *mRTPass;
    const engine::Camera *mCamera;
    RTRTLevel mLevel;
    std::vector<Animation*> mAnimations;
};

#endif