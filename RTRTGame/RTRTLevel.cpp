#include "RTRTLevel.h"
#include <Engine/Engine/RayTraceRenderPass.h>
#include <limits>

using namespace engine;
using namespace std;

RTRTLevel::RTRTLevel(void)
    : mLatestObjID(1)
{
}

RTRTLevel::~RTRTLevel(void)
{
    for (RTRTObject *o : mObjects) {
        delete o;
    }
}

void RTRTLevel::SetLevelFromPass(RayTraceRenderPass *rtPass)
{
    for (auto &o : rtPass->mSpheres) {
        mObjects.push_back(RTRTObjectFactory(o, mLatestObjID));
        ++mLatestObjID;
    }
    for (auto &o : rtPass->mRectangles) {
        mObjects.push_back(RTRTObjectFactory(o, mLatestObjID));
        ++mLatestObjID;
    }
    for (auto &o : rtPass->mBoxes) {
        mObjects.push_back(RTRTObjectFactory(o, mLatestObjID));
        ++mLatestObjID;
    }
    for (auto &o : rtPass->mLights) {
        mObjects.push_back(RTRTObjectFactory(o, mLatestObjID));
        ++mLatestObjID;
    }
}


void RTRTLevel::Update(float dt)
{
}

const RTRTObject* RTRTLevel::GetObject(int id) const
{
    for (const RTRTObject *o : mObjects) {
        if (o->GetID() == id) {
            return o;
        }
    }

    return nullptr;
}

RTRTObject* RTRTLevel::GetObject(int id)
{
    for (RTRTObject *o : mObjects) {
        if (o->GetID() == id) {
            return o;
        }
    }

    return nullptr;
}

RTRTObject* RTRTLevel::RayCast(const Ray &ray, float &lambda)
{
    RTRTObject *ret = nullptr;

    for (RTRTObject *o : mObjects) {
        if (o->Intersect(ray, lambda)) {
            ret = o;
        }
    }

    return ret;
}

RTRTObject* RTRTLevel::RayCast(const Ray &ray)
{
    float minLambda = numeric_limits<float>::max();
    return RayCast(ray, minLambda);
}
