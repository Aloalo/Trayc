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
        : mIntensity(intensity), mIsActive(isActive), mType(type)
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

    //---------- DirectionalLight ----------//

    DirectionalLight::DirectionalLight(const vec3 &intensity, const glm::vec3 &direction, bool isActive)
        : Light(intensity, isActive, Type::DIRECTIONAL), mDirection(normalize(direction))
    {
    }

    void DirectionalLight::ApplyToProgram(const Program *prog, const mat4 &V) const
    {
        prog->SetUniform("light.intensity", mIntensity);
        prog->SetUniform("light.direction", vec3(V * vec4(mDirection, 0.0f)));
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
        prog->SetUniform("light.position", vec3(V * vec4(mPosition, 0.0f)));
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
        prog->SetUniform("light.position", vec3(V * vec4(mPosition, 0.0f)));
        prog->SetUniform("light.spotDir", vec3(V * vec4(mSpotDirection, 0.0f)));
        prog->SetUniform("light.cosSpotCutoff", cos(radians(mSpotCutoff)));
        prog->SetUniform("light.spotExp", mSpotExponent);
    }

}
