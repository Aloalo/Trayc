#include "RTRTObjects.h"
#include <exception>

using namespace glm;
using namespace engine;
using namespace std;

RTRTObject::~RTRTObject()
{
}

RTRTSphere::RTRTSphere(RTSphere &object)
    : mObject(&object)
{
}

std::string RTRTSphere::Type() const
{
    return "RTSphere";
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

RTRTRectangle::RTRTRectangle(RTRectangle &object)
    : mObject(&object)
{
}


std::string RTRTRectangle::Type() const
{
    return "RTRectangle";
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

RTRTBox::RTRTBox(RTBox &object)
    : mObject(&object)
{
}

std::string RTRTBox::Type() const
{
    return "RTBox";
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

RTRTLight::RTRTLight(RTLight &object)
    : mObject(&object)
{
}

std::string RTRTLight::Type() const
{
    return "RTLight";
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
