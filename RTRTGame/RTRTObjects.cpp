#include "RTRTObjects.h"
#include <exception>

using namespace glm;
using namespace engine;
using namespace std;

const char* GetRTRTObjectTypeName(RTRTObjectType type)
{
    switch (type)
    {
    case RTRTObjectType::RTRTSphere:
        return "RTRTSphere";
    case RTRTObjectType::RTRTRectangle:
        return "RTRTRectangle";
    case RTRTObjectType::RTRTBox:
        return "RTRTBox";
    case RTRTObjectType::RTRTLight:
        return "RTRTLight";
    default:
        throw exception("Unknown RTRTObjectType");
    }
}

RTRTObject::RTRTObject(int id)
    : mID(id)
{
}

RTRTObject::~RTRTObject()
{
}

int RTRTObject::GetID() const
{
    return mID;
}

RTRTSphere::RTRTSphere(RTSphere &object, int id)
    : RTRTObject(id), mObject(&object)
{
}

RTRTObjectType RTRTSphere::Type() const
{
    return RTRTObjectType::RTRTSphere;
}

vec3 RTRTSphere::Position() const
{
    return vec3(mObject->positionRadius2);
}

void RTRTSphere::SetPosition(const vec3 &pos)
{
    mObject->positionRadius2 = vec4(pos, mObject->positionRadius2.w);
}

const void* RTRTSphere::Data(int &size) const
{
    size = sizeof(RTSphere);
    return static_cast<const void*>(mObject);
}

bool RTRTSphere::Intersect(const Ray &ray, float &minLambda) const
{
    return mObject->Intersect(ray, minLambda);
}

RTRTRectangle::RTRTRectangle(RTRectangle &object, int id)
    : RTRTObject(id), mObject(&object)
{
}


RTRTObjectType RTRTRectangle::Type() const
{
    return RTRTObjectType::RTRTRectangle;
}

vec3 RTRTRectangle::Position() const
{
    return 0.5f * (vec3(mObject->p1) + vec3(mObject->p3));
}

void RTRTRectangle::SetPosition(const vec3 &pos)
{
    const vec3 d = pos - Position();

    mObject->p1 = vec4(vec3(mObject->p1) + d, mObject->p1.w);
    mObject->p2 = vec4(vec3(mObject->p2) + d, mObject->p2.w);
    mObject->p3 = vec4(vec3(mObject->p3) + d, mObject->p3.w);
}

const void* RTRTRectangle::Data(int &size) const
{
    size = sizeof(RTRectangle);
    return static_cast<const void*>(mObject);
}

bool RTRTRectangle::Intersect(const Ray &ray, float &minLambda) const
{
    return mObject->Intersect(ray, minLambda);
}

RTRTBox::RTRTBox(RTBox &object, int id)
    : RTRTObject(id), mObject(&object)
{
}

RTRTObjectType RTRTBox::Type() const
{
    return RTRTObjectType::RTRTBox;
}

vec3 RTRTBox::Position() const
{
    return mObject->minv;
}

void RTRTBox::SetPosition(const vec3 &pos)
{
    const vec3 size = mObject->maxv - mObject->minv;
    mObject->minv = pos;
    mObject->maxv = pos + size;
}

const void* RTRTBox::Data(int &size) const
{
    size = sizeof(RTBox);
    return static_cast<const void*>(mObject);
}

bool RTRTBox::Intersect(const Ray &ray, float &minLambda) const
{
    return mObject->Intersect(ray, minLambda);
}

RTRTLight::RTRTLight(RTLight &object, int id)
    : RTRTObject(id), mObject(&object)
{
}

RTRTObjectType RTRTLight::Type() const
{
    return RTRTObjectType::RTRTLight;
}

vec3 RTRTLight::Position() const
{
    return vec3(mObject->positionRadius2);
}

void RTRTLight::SetPosition(const vec3 &pos)
{
    mObject->positionRadius2 = vec4(pos, mObject->positionRadius2.w);
}

const void* RTRTLight::Data(int &size) const
{
    size = sizeof(RTLight);
    return static_cast<const void*>(mObject);
}

bool RTRTLight::Intersect(const Ray &ray, float &minLambda) const
{
    return mObject->Intersect(ray, minLambda);
}
