/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef BP_VECTORFIELDS_H
#define BP_VECTORFIELDS_H

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <Engine/Core/Camera.h>


struct VectorField
{
    virtual ~VectorField(void)
    {
    }

    virtual glm::vec3 F(const glm::vec3 &p) const = 0;
};

struct Gravity : public VectorField
{
    Gravity(const glm::vec3 &gravity = glm::vec3(0.0f, -9.81f, 0.0f))
        : mGravity(gravity)
    {
    }

    virtual glm::vec3 F(const glm::vec3 &p) const override
    {
        return mGravity;
    }
private:
    glm::vec3 mGravity;
};

struct SphereAttractor : public VectorField
{
    SphereAttractor(const glm::vec3 &pos = glm::vec3(0.0f), float r = 10.0f)
        : mPosition(pos), mRadius(r)
    {
    }

    virtual glm::vec3 F(const glm::vec3 &p) const override
    {
        const glm::vec3 spherePoint = mPosition + glm::normalize(p - mPosition) * mRadius;
        return spherePoint - p;
    }
private:
    glm::vec3 mPosition;
    float mRadius;
};

struct SphereRepulsor : public VectorField
{
    SphereRepulsor(const glm::vec3 &pos = glm::vec3(0.0f), float r = 10.0f)
        : mPosition(pos), mRadius(r)
    {
    }

    virtual glm::vec3 F(const glm::vec3 &p) const override
    {
        const glm::vec3 spherePoint = mPosition + glm::normalize(p - mPosition) * mRadius;
        return p - spherePoint;
    }
private:
    glm::vec3 mPosition;
    float mRadius;
};

struct SphereRotator : public VectorField
{
    SphereRotator(const glm::vec3 &pos = glm::vec3(0.0f), float r = 10.0f)
        : mPosition(pos)
    {
    }

    virtual glm::vec3 F(const glm::vec3 &p) const override
    {
        static const glm::vec3 up(0.0f, 1.0f, 0.0f);
        return glm::normalize(glm::cross(mPosition - p, up) - 4.0f * (mPosition + p));
    }
private:
    glm::vec3 mPosition;
};

struct CameraGravity : public VectorField
{
    CameraGravity(const engine::Camera *cam, float mag = -9.81f)
        : mCamera(cam), mMag(mag)
    {
    }

    virtual glm::vec3 F(const glm::vec3 &p) const override
    {
        return mCamera->GetUp() * mMag;
    }

private:
    const engine::Camera *mCamera;
    float mMag;
};

#endif
