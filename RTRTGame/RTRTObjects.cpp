#include "RTRTObjects.h"
#include <exception>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace engine;
using namespace std;
using namespace jsonxx;

template<class T>
static inline Array VecToArray(const T &vec)
{
    Array ret;

    const int len = static_cast<int>(vec.length());
    for (int i = 0; i < len; ++i) {
        ret << vec[i];
    }

    return ret;
}

template<class T>
static inline T ArrayToVec(const Array &a)
{
    T ret;

    const int len = static_cast<int>(a.size());
    for (int i = 0; i < len; ++i) {
        ret[i] = static_cast<float>(a.get<Number>(i));
    }

    return ret;
}

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

RTRTObject* RTRTObjectFactory(const Object &object)
{
    const int id = static_cast<int>(object.get<Number>("ID"));
    const RTRTObjectType type = static_cast<RTRTObjectType>(static_cast<int>(object.get<Number>("type")));

    switch (type)
    {
    case RTRTObjectType::RTRTSphere:
    {
        RTSphere rtObject;
        rtObject.positionRadius2 = ArrayToVec<vec4>(object.get<Array>("positionRadius2"));
        rtObject.diffuseSpecular = ArrayToVec<vec4>(object.get<Array>("diffuseSpecular"));
        rtObject.materialData = ArrayToVec<vec2>(object.get<Array>("materialData"));
        rtObject.phi = static_cast<float>(object.get<Number>("phi"));
        rtObject.theta = static_cast<float>(object.get<Number>("theta"));

        return new RTRTSphere(rtObject, id);
    }
    case RTRTObjectType::RTRTRectangle:
    {
        RTRectangle rtObject;
        rtObject.p1 = ArrayToVec<vec4>(object.get<Array>("p1"));
        rtObject.p2 = ArrayToVec<vec4>(object.get<Array>("p2"));
        rtObject.p3 = ArrayToVec<vec4>(object.get<Array>("p3"));
        rtObject.diffuse = ArrayToVec<vec3>(object.get<Array>("diffuse"));

        return new RTRTRectangle(rtObject, id);
    }
    case RTRTObjectType::RTRTBox:
    {
        RTBox rtObject;
        rtObject.diffuseSpecular = ArrayToVec<vec4>(object.get<Array>("diffuseSpecular"));
        rtObject.minv = ArrayToVec<vec3>(object.get<Array>("minv"));
        rtObject.reflectivity = static_cast<float>(object.get<Number>("reflectivity"));
        rtObject.maxv = ArrayToVec<vec3>(object.get<Array>("maxv"));
        rtObject.gloss = static_cast<float>(object.get<Number>("gloss"));

        return new RTRTBox(rtObject, id);
    }
    case RTRTObjectType::RTRTLight:
    {
        RTLight rtObject;
        rtObject.positionRadius2 = ArrayToVec<vec4>(object.get<Array>("positionRadius2"));
        rtObject.intensity = ArrayToVec<vec3>(object.get<Array>("intensity"));

        return new RTRTLight(rtObject, id);
    }
    default:
        throw exception(("Invalid RTRTObjectType: " + to_string(static_cast<int>(type))).c_str());
    }

    return nullptr;
}

RTRTObject::RTRTObject(int id, float yaw)
    : mID(id), mYaw(yaw)
{
}

RTRTObject::~RTRTObject()
{
}

int RTRTObject::GetID() const
{
    return mID;
}

void RTRTObject::RotateYaw(float angle)
{
    throw exception("Calling unimplemented fuction RTRTObject::RotateYaw");
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

Object RTRTSphere::GetJsonObject() const
{
    Object ret;
    ret << "ID" << mID;
    ret << "type" << static_cast<int>(Type());
    ret << "positionRadius2" << VecToArray(mObject->positionRadius2);
    ret << "diffuseSpecular" << VecToArray(mObject->diffuseSpecular);
    ret << "materialData" << VecToArray(mObject->materialData);
    ret << "phi" << mObject->phi;
    ret << "theta" << mObject->theta;
    return ret;
}

RTRTRectangle::RTRTRectangle(RTRectangle &object, int id, float yaw)
    : RTRTObject(id), mObject(&object)
{
    RotateYaw(mYaw);
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

void RTRTRectangle::RotateYaw(float angle)
{
    mYaw += angle;

    const mat4 R = rotate(mat4(1.0f), angle, vec3(0.0f, 1.0f, 0.0f));
    const vec3 pos = Position();
    SetPosition(vec3(0.0f));

    mObject->p1 = vec4(vec3(R * vec4(vec3(mObject->p1), 1.0f)), mObject->p1.w);
    mObject->p2 = vec4(vec3(R * vec4(vec3(mObject->p2), 1.0f)), mObject->p2.w);
    mObject->p3 = vec4(vec3(R * vec4(vec3(mObject->p3), 1.0f)), mObject->p3.w);

    SetPosition(pos);
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

Object RTRTRectangle::GetJsonObject() const
{
    Object ret;
    ret << "ID" << mID;
    ret << "type" << static_cast<int>(Type());
    ret << "p1" << VecToArray(mObject->p1);
    ret << "p2" << VecToArray(mObject->p2);
    ret << "p3" << VecToArray(mObject->p3);
    ret << "diffuse" << VecToArray(mObject->diffuse);
    return ret;
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

Object RTRTBox::GetJsonObject() const
{
    Object ret;
    ret << "ID" << mID;
    ret << "type" << static_cast<int>(Type());
    ret << "diffuseSpecular" << VecToArray(mObject->diffuseSpecular);
    ret << "minv" << VecToArray(mObject->minv);
    ret << "reflectivity" << mObject->reflectivity;
    ret << "maxv" << VecToArray(mObject->maxv);
    ret << "gloss" << mObject->gloss;
    return ret;
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

Object RTRTLight::GetJsonObject() const
{
    Object ret;
    ret << "ID" << mID;
    ret << "type" << static_cast<int>(Type());
    ret << "positionRadius2" << VecToArray(mObject->positionRadius2);
    ret << "intensity" << VecToArray(mObject->intensity);
    return ret;
}
