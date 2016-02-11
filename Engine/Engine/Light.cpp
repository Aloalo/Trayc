/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Engine/Light.h>

using namespace glm;

namespace engine
{
    Light::Light(const vec3 &intensity, LightType type, bool isActive)
        : mIntensity(intensity), mType(type), mIsActive(isActive)
    {
    }

    Light::~Light()
    {
    }

    void Light::ApplyToProgram(const Program &prog) const
    {
        prog.SetUniform("Light.intensity", mIntensity);
    }

    bool Light::operator<(const Light *other) const
    {
        return mType < other->mType;
    }

    DirectionalLight::DirectionalLight(const vec3 &intensity, bool isActive)
        : Light(intensity, LightType::DIRECTIONAL, isActive)
    {
    }

    void DirectionalLight::ApplyToProgram(const Program &prog) const
    {
        Light::ApplyToProgram(prog);
    }

    PointLight::PointLight(const vec3 &intensity, bool isActive, const vec3 &attenuation, const vec3 &position)
        : Light(intensity, LightType::POINT, isActive), mAttenuation(attenuation), mPosition(position)
    {
    }

    void PointLight::ApplyToProgram(const Program &prog) const
    {
        Light::ApplyToProgram(prog);
        prog.SetUniform("Light.attenuation", mAttenuation);
        prog.SetUniform("Light.position", mPosition);
    }

    SpotLight::SpotLight(const vec3 &intensity, bool isActive, const vec3 &attenuation, const vec3 &position, const vec3 &spotDir, float spotCutoff, float spotExp)
        : Light(intensity, LightType::SPOT, isActive), mAttenuation(attenuation), mPosition(position), mSpotDirection(spotDir), mSpotCutoff(spotCutoff), mSpotExponent(spotExp)
    {
    }

    void SpotLight::ApplyToProgram(const Program &prog) const
    {
        Light::ApplyToProgram(prog);
        prog.SetUniform("Light.attenuation", mAttenuation);
        prog.SetUniform("Light.position", mPosition);
        prog.SetUniform("Light.spotDir", mSpotDirection);
        prog.SetUniform("Light.spotCutoff", mSpotCutoff);
        prog.SetUniform("Light.spotExp", mSpotExponent);
    }

}
