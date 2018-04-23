#include "RTRTObjects.h"
#include <exception>

using namespace glm;
using namespace engine;
using namespace std;

RTRTObject::~RTRTObject()
{
}

vec3 RTRTSphere::Position() const
{
    return vec3(object.positionRadius);
}

void RTRTSphere::SetPosition(const vec3 &pos)
{
    object.positionRadius = vec4(pos, object.positionRadius.w);
}

const void* RTRTSphere::Data(int &size) const
{
    size = sizeof(object);
    return reinterpret_cast<const void*>(&object);
}

vec3 RTRTRectangle::Position() const
{
    switch (object.normal)
    {
    case 0:
        return vec3(object.offset, vec2(object.rect));
    case 1:
        return vec3(object.rect.y, object.offset, object.rect.x);
    case 2:
        return vec3(vec2(object.rect), object.offset);
    default:
        throw exception("Invalid rectangle normal: " + object.normal);
    }
}

void RTRTRectangle::SetPosition(const vec3 &pos)
{
    const vec2 size = vec2(object.rect.z, object.rect.w) - vec2(object.rect);

    switch (object.normal)
    {
    case 0:
    {
        object.offset = pos.x;
        const vec2 minv = vec2(pos.y, pos.z);
        object.rect = vec4(minv, minv + size);
        break;
    }
    case 1:
    {
        // X-Z plane is swizzled!
        object.offset = pos.y;
        const vec2 minv = vec2(pos.z, pos.x);
        object.rect = vec4(minv, minv + size);
        break;
    }
    case 2:
    {
        object.offset = pos.z;
        const vec2 minv = vec2(pos.x, pos.y);
        object.rect = vec4(minv, minv + size);
        break;
    }
    default:
        throw exception("Invalid rectangle normal: " + object.normal);
    }
}

const void* RTRTRectangle::Data(int &size) const
{
    size = sizeof(object);
    return reinterpret_cast<const void*>(&object);
}

vec3 RTRTBox::Position() const
{
    return object.minv;
}

void RTRTBox::SetPosition(const vec3 &pos)
{
    const vec3 size = object.maxv - object.minv;
    object.minv = pos;
    object.maxv = pos + size;
}

const void* RTRTBox::Data(int &size) const
{
    size = sizeof(object);
    return reinterpret_cast<const void*>(&object);
}

vec3 RTRTLight::Position() const
{
    return vec3(object.positionRadius);
}

void RTRTLight::SetPosition(const vec3 &pos)
{
    object.positionRadius = vec4(pos, object.positionRadius.w);
}

const void* RTRTLight::Data(int &size) const
{
    size = sizeof(object);
    return reinterpret_cast<const void*>(&object);
}
