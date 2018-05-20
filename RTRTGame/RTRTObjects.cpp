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
    return vec3(mObject->positionRadius);
}

void RTRTSphere::SetPosition(const vec3 &pos)
{
    mObject->positionRadius = vec4(pos, mObject->positionRadius.w);
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
    switch (mObject->normal)
    {
    case 0:
        return vec3(mObject->offset, vec2(mObject->rect));
    case 1:
        return vec3(mObject->rect.y, mObject->offset, mObject->rect.x);
    case 2:
        return vec3(vec2(mObject->rect), mObject->offset);
    default:
        throw exception("Invalid rectangle normal: " + mObject->normal);
    }
}

void RTRTRectangle::SetPosition(const vec3 &pos)
{
    const vec2 size = vec2(mObject->rect.z, mObject->rect.w) - vec2(mObject->rect);

    switch (mObject->normal)
    {
    case 0:
    {
        mObject->offset = pos.x;
        const vec2 minv = vec2(pos.y, pos.z);
        mObject->rect = vec4(minv, minv + size);
        break;
    }
    case 1:
    {
        // X-Z plane is swizzled!
        mObject->offset = pos.y;
        const vec2 minv = vec2(pos.z, pos.x);
        mObject->rect = vec4(minv, minv + size);
        break;
    }
    case 2:
    {
        mObject->offset = pos.z;
        const vec2 minv = vec2(pos.x, pos.y);
        mObject->rect = vec4(minv, minv + size);
        break;
    }
    default:
        throw exception("Invalid rectangle normal: " + mObject->normal);
    }
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
    return vec3(mObject->positionRadius);
}

void RTRTLight::SetPosition(const vec3 &pos)
{
    mObject->positionRadius = vec4(pos, mObject->positionRadius.w);
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
