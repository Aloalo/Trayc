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
    virtual ~RTRTObject();

    virtual std::string Type() const = 0;
    virtual glm::vec3 Position() const = 0;
    virtual void SetPosition(const glm::vec3 &pos) = 0;
    virtual const void* Data(int &size) const = 0;
};


class RTRTSphere : public RTRTObject
{
public:
    RTRTSphere(const engine::RTSphere &object);

    virtual std::string Type() const override;
    virtual glm::vec3 Position() const override;
    virtual void SetPosition(const glm::vec3 &pos) override;
    virtual const void* Data(int &size) const override;

private:
    engine::RTSphere object;
};


class RTRTRectangle : public RTRTObject
{
public:
    RTRTRectangle(const engine::RTRectangle &object);

    virtual std::string Type() const override;
    virtual glm::vec3 Position() const override;
    virtual void SetPosition(const glm::vec3 &pos) override;
    virtual const void* Data(int &size) const override;

private:
    engine::RTRectangle object;
};


class RTRTBox : public RTRTObject
{
public:
    RTRTBox(const engine::RTBox &object);

    virtual std::string Type() const override;
    virtual glm::vec3 Position() const override;
    virtual void SetPosition(const glm::vec3 &pos) override;
    virtual const void* Data(int &size) const override;

private:
    engine::RTBox object;
};


class RTRTLight : public RTRTObject
{
public:
    RTRTLight(const engine::RTLight &object);

    virtual std::string Type() const override;
    virtual glm::vec3 Position() const override;
    virtual void SetPosition(const glm::vec3 &pos) override;
    virtual const void* Data(int &size) const override;

private:
    engine::RTLight object;
};


template<class T>
inline RTRTObject* RTRTObjectFactory(const T &object)
{
    return nullptr;
}

template<>
inline RTRTObject* RTRTObjectFactory<engine::RTSphere>(const engine::RTSphere &object)
{
    return new RTRTSphere(object);
}

template<>
inline RTRTObject* RTRTObjectFactory<engine::RTRectangle>(const engine::RTRectangle &object)
{
    return new RTRTRectangle(object);
}

template<>
inline RTRTObject* RTRTObjectFactory<engine::RTBox>(const engine::RTBox &object)
{
    return new RTRTBox(object);
}

template<>
inline RTRTObject* RTRTObjectFactory<engine::RTLight>(const engine::RTLight &object)
{
    return new RTRTLight(object);
}

#endif
