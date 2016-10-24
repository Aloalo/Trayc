/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Engine/Light.h>
#include <Engine/GL/Program.h>

using namespace glm;
using namespace std;

namespace engine
{
    //---------- Light ----------//
    Light::Light(const vec3 &intensity, bool isActive, Type type)
        : mIntensity(intensity), mIsActive(isActive), mType(type), mShadowmap(nullptr)
    {
    }

    Light::~Light()
    {
    }

    bool Light::operator<(const Light *other) const
    {
        return mType < other->mType;
    }

    Light::Type Light::GetType() const
    {
        return mType;
    }

    const Texture * Light::GetShadowmap() const
    {
        return mShadowmap;
    }


    //---------- AmbientLight ----------//
    AmbientLight::AmbientLight(const vec3 &intensity, bool isActive)
        : Light(intensity, isActive, Type::AMBIENT)
    {
    }

    void AmbientLight::ApplyToProgram(const Program *prog, const mat4 &V) const
    {
        prog->SetUniform("light.intensity", mIntensity);
    }


    //---------- DirectionalLight ----------//
    DirectionalLight::DirectionalLight(const vec3 &intensity, bool isActive, const glm::vec3 &direction)
        : Light(intensity, isActive, Type::DIRECTIONAL), mDirection(normalize(direction))
    {
    }

    void DirectionalLight::ApplyToProgram(const Program *prog, const mat4 &V) const
    {
        prog->SetUniform("light.intensity", mIntensity);
        prog->SetUniform("light.direction", normalize(vec3(V * vec4(mDirection, 0.0f))));
    }

    void DirectionalLight::SetDirection(const vec3 &direction)
    {
        mDirection = direction;
    }

    const vec3& DirectionalLight::GetDirection() const
    {
        return mDirection;
    }

    //---------- GlobalLight ----------//
    GlobalLight::GlobalLight(const vec3 &ambientIntensity, const vec3 &directionalIntensity, bool isActive, const vec3 &direction)
        : Light(ambientIntensity, isActive, Type::GLOBAL_LIGHT), mDirectionalIntensity(directionalIntensity), mDirection(normalize(direction))
    {
    }

    void GlobalLight::ApplyToProgram(const Program *prog, const mat4 &V) const
    {
        prog->SetUniform("light.aIntensity", mIntensity);
        prog->SetUniform("light.intensity", mDirectionalIntensity);
        prog->SetUniform("light.direction", normalize(vec3(V * vec4(mDirection, 0.0f))));
    }

    void GlobalLight::SetDirection(const vec3 &direction)
    {
        mDirection = direction;
    }

    const vec3& GlobalLight::GetDirection() const
    {
        return mDirection;
    }


    //---------- PointLight ----------//
    PointLight::PointLight(const vec3 &intensity, bool isActive, const vec3 &attenuation, const vec3 &position)
        : Light(intensity, isActive, Type::POINT), mAttenuation(attenuation), mPosition(position)
    {
    }

    void PointLight::ApplyToProgram(const Program *prog, const mat4 &V) const
    {
        prog->SetUniform("light.intensity", mIntensity);
        prog->SetUniform("light.attenuation", mAttenuation);
        prog->SetUniform("light.position", vec3(V * vec4(mPosition, 1.0f)));
    }

    void PointLight::SetPosition(const vec3 &position)
    {
        mPosition = position;
    }

    const vec3& PointLight::GetPosition() const
    {
        return mPosition;
    }


    //---------- SpotLight ----------//
    SpotLight::SpotLight(const vec3 &intensity, bool isActive, const vec3 &attenuation, const vec3 &position, const vec3 &spotDir, float spotCutoff, float spotExp)
        : Light(intensity, isActive, Type::SPOT), mAttenuation(attenuation), mPosition(position), mSpotDirection(normalize(spotDir)), mSpotCutoff(spotCutoff), mSpotExponent(spotExp)
    {
    }

    void SpotLight::ApplyToProgram(const Program *prog, const mat4 &V) const
    {
        prog->SetUniform("light.intensity", mIntensity);
        prog->SetUniform("light.attenuation", mAttenuation);
        prog->SetUniform("light.position", vec3(V * vec4(mPosition, 1.0f)));
        prog->SetUniform("light.spotDir", normalize(vec3(V * vec4(mSpotDirection, 0.0f))));
        prog->SetUniform("light.cosSpotCutoff", cosf(radians(mSpotCutoff)));
        prog->SetUniform("light.spotExp", mSpotExponent);
    }

    void SpotLight::SetPosition(const vec3 &position)
    {
        mPosition = position;
    }

    const vec3& SpotLight::GetPosition() const
    {
        return mPosition;
    }

    void SpotLight::SetDirection(const vec3 &direction)
    {
        mSpotDirection = direction;
    }

    const vec3& SpotLight::GetDirection() const
    {
        return mSpotDirection;
    }
}
