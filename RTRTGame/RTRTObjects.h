#ifndef RTRT_RTRT_OBJECTS_H
#define RTRT_RTRT_OBJECTS_H

#include <glm/glm.hpp>
#include <Engine/Geometry/RayTracePrimitives.h>
#include <string>
#include <jsonxx/jsonxx.h>

enum class RTRTObjectType
{
    RTRTSphere,
    RTRTRectangle,
    RTRTBox,
    RTRTLight
};

static const RTRTObjectType RTRT_OBJECT_TYPES[] = { 
    RTRTObjectType::RTRTSphere ,
    RTRTObjectType::RTRTRectangle,
    RTRTObjectType::RTRTBox,
    RTRTObjectType::RTRTLight
};

const char* GetRTRTObjectTypeName(RTRTObjectType type);

class RTRTObject
{
public:
    RTRTObject(int id, float yaw = 0.0f);
    virtual ~RTRTObject();

    int GetID() const;

    virtual RTRTObjectType Type() const = 0;
    virtual glm::vec3 Position() const = 0;
    virtual void SetPosition(const glm::vec3 &pos) = 0;
    virtual void RotateYaw(float angle);
    virtual const void* Data(int &size) const = 0;
    virtual bool Intersect(const engine::Ray &ray, float &minLambda) const = 0;
    virtual jsonxx::Object GetJsonObject() const = 0;

protected:
    int mID;
    float mYaw;
};


class RTRTSphere : public RTRTObject
{
public:
    RTRTSphere(engine::RTSphere &object, int id);

    virtual RTRTObjectType Type() const override;
    virtual glm::vec3 Position() const override;
    virtual void SetPosition(const glm::vec3 &pos) override;
    virtual const void* Data(int &size) const override;
    virtual bool Intersect(const engine::Ray &ray, float &minLambda) const override;
    virtual jsonxx::Object GetJsonObject() const override;

private:
    engine::RTSphere *mObject;
};


class RTRTRectangle : public RTRTObject
{
public:
    RTRTRectangle(engine::RTRectangle &object, int id, float yaw = 0.0f);

    virtual RTRTObjectType Type() const override;
    virtual glm::vec3 Position() const override;
    virtual void SetPosition(const glm::vec3 &pos) override;
    virtual void RotateYaw(float angle) override;
    virtual const void* Data(int &size) const override;
    virtual bool Intersect(const engine::Ray &ray, float &minLambda) const override;
    virtual jsonxx::Object GetJsonObject() const override;

private:
    engine::RTRectangle *mObject;
    
};


class RTRTBox : public RTRTObject
{
public:
    RTRTBox(engine::RTBox &object, int id);

    virtual RTRTObjectType Type() const override;
    virtual glm::vec3 Position() const override;
    virtual void SetPosition(const glm::vec3 &pos) override;
    virtual const void* Data(int &size) const override;
    virtual bool Intersect(const engine::Ray &ray, float &minLambda) const override;
    virtual jsonxx::Object GetJsonObject() const override;

private:
    engine::RTBox *mObject;
};


class RTRTLight : public RTRTObject
{
public:
    RTRTLight(engine::RTLight &object, int id);

    virtual RTRTObjectType Type() const override;
    virtual glm::vec3 Position() const override;
    virtual void SetPosition(const glm::vec3 &pos) override;
    virtual const void* Data(int &size) const override;
    virtual bool Intersect(const engine::Ray &ray, float &minLambda) const override;
    virtual jsonxx::Object GetJsonObject() const override;

private:
    engine::RTLight *mObject;
};

RTRTObject* RTRTObjectFactory(const jsonxx::Object &object);

template<class T>
inline RTRTObject* RTRTObjectFactory(T &object, int id);

template<>
inline RTRTObject* RTRTObjectFactory<engine::RTSphere>(engine::RTSphere &object, int id)
{
    return new RTRTSphere(object, id);
}

template<>
inline RTRTObject* RTRTObjectFactory<engine::RTRectangle>(engine::RTRectangle &object, int id)
{
    return new RTRTRectangle(object, id);
}

template<>
inline RTRTObject* RTRTObjectFactory<engine::RTBox>(engine::RTBox &object, int id)
{
    return new RTRTBox(object, id);
}

template<>
inline RTRTObject* RTRTObjectFactory<engine::RTLight>(engine::RTLight &object, int id)
{
    return new RTRTLight(object, id);
}

#endif
