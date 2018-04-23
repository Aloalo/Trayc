#ifndef RTRT_RTRT_OBJECTS_H
#define RTRT_RTRT_OBJECTS_H

#define GLM_SWIZZLE_XYZW
#include <glm/glm.hpp>
#include <Engine/Geometry/RayTracePrimitives.h>

class RTRTObject
{
public:
    virtual ~RTRTObject();

    virtual glm::vec3 Position() const = 0;
    virtual void SetPosition(const glm::vec3 &pos) = 0;
    virtual const void* Data(int &size) const = 0;
};


class RTRTSphere : public RTRTObject
{
public:
    virtual glm::vec3 Position() const override;
    virtual void SetPosition(const glm::vec3 &pos) override;
    virtual const void* Data(int &size) const override;

private:
    engine::RTSphere object;
};


class RTRTRectangle : public RTRTObject
{
public:
    virtual glm::vec3 Position() const override;
    virtual void SetPosition(const glm::vec3 &pos) override;
    virtual const void* Data(int &size) const override;

private:
    engine::RTRectangle object;
};


class RTRTBox : public RTRTObject
{
public:
    virtual glm::vec3 Position() const override;
    virtual void SetPosition(const glm::vec3 &pos) override;
    virtual const void* Data(int &size) const override;

private:
    engine::RTBox object;
};


class RTRTLight : public RTRTObject
{
public:
    virtual glm::vec3 Position() const override;
    virtual void SetPosition(const glm::vec3 &pos) override;
    virtual const void* Data(int &size) const override;

private:
    engine::RTLight object;
};

#endif
