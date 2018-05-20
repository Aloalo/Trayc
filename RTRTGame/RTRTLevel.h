#ifndef RTRT_RTRT_LEVEL_H
#define RTRT_RTRT_LEVEL_H

#include <vector>

#include "RTRTObjects.h"

namespace engine
{
    class RayTraceRenderPass;
}

class RTRTLevel
{
public:
    RTRTLevel(void);
    ~RTRTLevel(void);

    void SetLevelFromPass(engine::RayTraceRenderPass *rtPass);
    void Update(float dt);
    const RTRTObject* GetObject(int id) const;
    RTRTObject* GetObject(int id);

    RTRTObject* RayCast(const engine::Ray &ray, float &lambda);
    RTRTObject* RayCast(const engine::Ray &ray);

    std::string mName = "default";
    std::vector<RTRTObject*> mObjects;

private:
    int mLatestObjID;
};

#endif
