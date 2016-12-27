
#include <Engine/Engine/Light.h>
#include <Engine/GL/Program.h>
#include <Engine/Core/Camera.h>
#include <Engine/Geometry/Object3D.h>
#include <easylogging++.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

using namespace glm;
using namespace std;

namespace engine
{
    static inline mat4 CalcDirectionalShadowVP(const AABB &aabb, const vec3 &viewDir)
    {
        static const vec3 up = vec3(0.0f, 1.0f, 0.0f);
        const vec3 pos = aabb.Center();
        const mat4 V = lookAt(pos, pos + viewDir, up);

        vec3 minimum = aabb.mMinv;
        vec3 maximum = aabb.mMaxv;
        const float width = maximum.x - minimum.x;
        const float depth = maximum.z - minimum.z;

        array<vec3, 8> vert;
        vert[0] = minimum;
        vert[1] = vec3(minimum.x + width, minimum.y, minimum.z);
        vert[2] = vec3(minimum.x + width, minimum.y, minimum.z + depth);
        vert[3] = vec3(minimum.x, minimum.y, minimum.z + depth);
        vert[4] = vec3(maximum.x - width, maximum.y, maximum.z - depth);
        vert[5] = vec3(maximum.x, maximum.y, maximum.z - depth);
        vert[6] = maximum;
        vert[7] = vec3(maximum.x - width, maximum.y, maximum.z);

        AABB orthographicAABB;
        for(const vec3 &v : vert) {
            orthographicAABB |= vec3((V * vec4(v, 1.0f)));
        }

        minimum = orthographicAABB.mMinv;
        maximum = orthographicAABB.mMaxv;
        const mat4 P = ortho(minimum.x, maximum.x, minimum.y, maximum.y, minimum.z, maximum.z);

        return P * V;
    }


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

    const Texture* Light::GetProjectedShadow() const
    {
        return mProjectedShadow;
    }

    void Light::ApplyToShadowProgram(const Program * prog, const glm::mat4 & V) const
    {
        LOG(WARNING) << "[Light::ApplyToShadowProgram] Calling unimplemented function.";
    }

    mat4 Light::GetDepthBiasVP(const AABB &aabb) const
    {
        static const mat4 biasMatrix(
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
        );

        return biasMatrix * GetDepthVP(aabb);
    }

    bool Light::Affects(const Object3D * obj) const
    {
        return true;
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

    mat4 AmbientLight::GetDepthVP(const AABB &aabb) const
    {
        return mat4(1.0f);
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

    void DirectionalLight::ApplyToShadowProgram(const Program * prog, const mat4 &V) const
    {
        prog->SetUniform("light.direction", normalize(vec3(V * vec4(mDirection, 0.0f))));
    }

    mat4 DirectionalLight::GetDepthVP(const AABB &aabb) const
    {
        return CalcDirectionalShadowVP(aabb, -mDirection);
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

    void GlobalLight::ApplyToShadowProgram(const Program * prog, const glm::mat4 & V) const
    {
        prog->SetUniform("light.direction", normalize(vec3(V * vec4(mDirection, 0.0f))));
    }

    mat4 GlobalLight::GetDepthVP(const AABB &aabb) const
    {
        return CalcDirectionalShadowVP(aabb, -mDirection);
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

    mat4 PointLight::GetDepthVP(const AABB &aabb) const
    {
        return mat4(1.0f);
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
    static const float farRange = 10000.0f;
    SpotLight::SpotLight(const vec3 &intensity, bool isActive, const vec3 &attenuation, const vec3 &position, const vec3 &spotDir, float spotCutoff, float spotExp)
        : Light(intensity, isActive, Type::SPOT), mAttenuation(attenuation), mPosition(position), mSpotDirection(normalize(spotDir)), mSpotCutoff(spotCutoff), mSpotExponent(spotExp)
    {
        const vec3 center = mPosition + GetDirection() * farRange * 0.5f;
        mFrustum.Set(mPosition, center, vec3(0.0f, 1.0f, 0.0f), mSpotCutoff * 2.0f, 1.0f, 10.0f, farRange);
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

    void SpotLight::ApplyToShadowProgram(const Program * prog, const glm::mat4 & V) const
    {
        prog->SetUniform("light.position", vec3(V * vec4(mPosition, 1.0f)));
    }

    mat4 SpotLight::GetDepthVP(const AABB &aabb) const
    {
        const vec3 sceneSize = aabb.Size();
        const float maxSceneSize = max(sceneSize.x, max(sceneSize.y, sceneSize.z)) + 0.001f;
        const float farDist = maxSceneSize + distance(aabb.Center(), mPosition);

        Camera cam(GetPosition(), 1.0f, 2.0f * GetSpotCutoff(), 10.0f, farDist);
        cam.SetDirection(GetDirection());
        const mat4 depthViewMatrix = cam.GetViewMatrix();
        const mat4 depthProjectionMatrix = cam.GetProjectionMatrix();
        return depthProjectionMatrix * depthViewMatrix;
    }

    bool SpotLight::Affects(const Object3D *obj) const
    {
        return mFrustum.Intersect(obj->GetBSphere()) && mFrustum.Intersect(obj->GetAABB());
    }

    void SpotLight::SetPosition(const vec3 &position)
    {
        mPosition = position;
        const vec3 center = mPosition + GetDirection() * farRange * 0.5f;
        mFrustum.Set(mPosition, center, vec3(0.0f, 1.0f, 0.0f), mSpotCutoff * 2.0f, 1.0f, 10.0f, farRange);
    }

    const vec3& SpotLight::GetPosition() const
    {
        return mPosition;
    }

    void SpotLight::SetDirection(const vec3 &direction)
    {
        mSpotDirection = direction;
        const vec3 center = mPosition + GetDirection() * farRange * 0.5f;
        mFrustum.Set(mPosition, center, vec3(0.0f, 1.0f, 0.0f), mSpotCutoff * 2.0f, 1.0f, 10.0f, farRange);
    }

    const vec3& SpotLight::GetDirection() const
    {
        return mSpotDirection;
    }

    float SpotLight::GetSpotCutoff() const
    {
        return mSpotCutoff;
    }
}
