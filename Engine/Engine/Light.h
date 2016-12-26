
#ifndef EN_LIGHT_H
#define EN_LIGHT_H

#include <glm/glm.hpp>
#include <Engine/Engine/GlobalRenderingParams.h>
#include <Engine/Geometry/Frustum.h>

namespace engine
{
    class Program;
    class Texture;
    struct Object3D;

    //---------- Light ----------//
    struct Light
    {
        enum Type
        {
            AMBIENT = 0,
            DIRECTIONAL = 1,
            GLOBAL_LIGHT = 2,
            POINT = 3,
            SPOT = 4,

            CT_LIGHT_TYPES = 5
        };

        Light(const glm::vec3 &intensity, bool isActive, Type type);
        virtual ~Light();

        bool operator<(const Light* other) const;
        Type GetType() const;
        const Texture* GetShadowmap() const;
        // Params need to be transformed to view space
        virtual void ApplyToProgram(const Program *prog, const glm::mat4 &V) const = 0;
        virtual void ApplyToShadowProgram(const Program *prog, const glm::mat4 &V) const;

        // Shadowmapping transforms
        virtual glm::mat4 GetDepthVP(const AABB &aabb) const = 0;
        glm::mat4 GetDepthBiasVP(const AABB &aabb) const;

        virtual bool Affects(const Object3D *obj) const;

        glm::vec3 mIntensity;

        bool mIsActive;

    private:
        friend class ShadowRenderPass;
        Type mType;
        const Texture *mShadowmap;
    };

    //---------- AmbientLight ----------//
    struct AmbientLight : public Light
    {
        AmbientLight(const glm::vec3 &intensity, bool isActive);

        virtual void ApplyToProgram(const Program *prog, const glm::mat4 &V) const override;
        virtual glm::mat4 GetDepthVP(const AABB &aabb) const override;
    };


    //---------- DirectionalLight ----------//
    struct DirectionalLight : public Light
    {
        DirectionalLight(const glm::vec3 &intensity, bool isActive, const glm::vec3 &direction);

        virtual void ApplyToProgram(const Program *prog, const glm::mat4 &V) const override;
        virtual void ApplyToShadowProgram(const Program *prog, const glm::mat4 &V) const override;
        virtual glm::mat4 GetDepthVP(const AABB &aabb) const override;

        void SetDirection(const glm::vec3 &direction);
        const glm::vec3& GetDirection() const;

    private:
        glm::vec3 mDirection;
    };

    //---------- GlobalLight ----------// AMBIENT + DIRECTIONAL
    struct GlobalLight : public Light
    {
        GlobalLight(const glm::vec3 &ambientIntensity, const glm::vec3 &directionalIntensity, bool isActive, const glm::vec3 &direction);

        virtual void ApplyToProgram(const Program *prog, const glm::mat4 &V) const override;
        virtual void ApplyToShadowProgram(const Program *prog, const glm::mat4 &V) const override;
        virtual glm::mat4 GetDepthVP(const AABB &aabb) const override;

        void SetDirection(const glm::vec3 &direction);
        const glm::vec3& GetDirection() const;

    private:
        glm::vec3 mDirectionalIntensity;
        glm::vec3 mDirection;
    };


    //---------- PointLight ----------//
    struct PointLight : public Light
    {
        PointLight(const glm::vec3 &intensity, 
            bool isActive,
            const glm::vec3 &attenuation,
            const glm::vec3 &position);

        virtual void ApplyToProgram(const Program *prog, const glm::mat4 &V) const override;
        virtual glm::mat4 GetDepthVP(const AABB &aabb) const override;

        void SetPosition(const glm::vec3 &position);
        const glm::vec3& GetPosition() const;

    private:
        //Constant, linear, quadratic
        glm::vec3 mAttenuation;
        //This is direction is the light is directional
        glm::vec3 mPosition;
    };


    //---------- SpotLight ----------//
    struct SpotLight : public Light
    {
        SpotLight(const glm::vec3 &intensity,
            bool isActive,
            const glm::vec3 &attenuation,
            const glm::vec3 &position,
            const glm::vec3 &spotDir,
            float spotCutoff,
            float spotExp);

        virtual void ApplyToProgram(const Program *prog, const glm::mat4 &V) const override;
        virtual void ApplyToShadowProgram(const Program *prog, const glm::mat4 &V) const override;
        virtual glm::mat4 GetDepthVP(const AABB &aabb) const override;
        virtual bool Affects(const Object3D *obj) const override;

        void SetPosition(const glm::vec3 &position);
        const glm::vec3& GetPosition() const;

        void SetDirection(const glm::vec3 &direction);
        const glm::vec3& GetDirection() const;

        float GetSpotCutoff() const;

    private:
        //Constant, linear, quadratic
        glm::vec3 mAttenuation;
        //This is direction is the light is directional
        glm::vec3 mPosition;
        // SpotLight specific
        glm::vec3 mSpotDirection;
        float mSpotCutoff;
        float mSpotExponent;

        Frustum mFrustum;
    };
}

#endif
