#ifndef RTRT_RTRT_OBJECTS_H
#define RTRT_RTRT_OBJECTS_H

#define GLM_SWIZZLE_XYZW
#include <glm/glm.hpp>
#include <Engine/Geometry/RayTracePrimitives.h>
#include <string>
#include <type_traits>

static const std::string OBJECT_TYPES[] = {
    "RTSphere", "RTRectangle", "RTBox", "RTLight"
};

class RTRTObject
{
public:
    RTRTObject(int id);
    virtual ~RTRTObject();

    int GetID() const;

    virtual std::string Type() const = 0;
    virtual glm::vec3 Position() const = 0;
    virtual void SetPosition(const glm::vec3 &pos) = 0;
    virtual const void* Data(int &size) const = 0;

protected:
    int mID;
};


class RTRTSphere : public RTRTObject
{
public:
    RTRTSphere(engine::RTSphere &object, int id);

    virtual std::string Type() const override;
    virtual glm::vec3 Position() const override;
    virtual void SetPosition(const glm::vec3 &pos) override;
    virtual const void* Data(int &size) const override;

private:
    engine::RTSphere *mObject;
};


class RTRTRectangle : public RTRTObject
{
public:
    RTRTRectangle(engine::RTRectangle &object, int id);

    virtual std::string Type() const override;
    virtual glm::vec3 Position() const override;
    virtual void SetPosition(const glm::vec3 &pos) override;
    virtual const void* Data(int &size) const override;

private:
    engine::RTRectangle *mObject;
};


class RTRTBox : public RTRTObject
{
public:
    RTRTBox(engine::RTBox &object, int id);

    virtual std::string Type() const override;
    virtual glm::vec3 Position() const override;
    virtual void SetPosition(const glm::vec3 &pos) override;
    virtual const void* Data(int &size) const override;

private:
    engine::RTBox *mObject;
};


class RTRTLight : public RTRTObject
{
public:
    RTRTLight(engine::RTLight &object, int id);

    virtual std::string Type() const override;
    virtual glm::vec3 Position() const override;
    virtual void SetPosition(const glm::vec3 &pos) override;
    virtual const void* Data(int &size) const override;

private:
    engine::RTLight *mObject;
};

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
