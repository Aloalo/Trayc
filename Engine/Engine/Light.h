
#ifndef EN_LIGHT_H
#define EN_LIGHT_H

#include <glm/glm.hpp>
#include <Engine/Engine/GlobalRenderingParams.h>

namespace engine
{
    class Program;

    //---------- Light ----------//
    struct Light
    {
        enum Type
        {
            DIRECTIONAL = 0,
            POINT = 1,
            SPOT = 2,
            AMBIENT = 3,

            CT_LIGHT_TYPES
        };

        Light(const glm::vec3 &intensity, bool isActive, Type type);
        virtual ~Light();

        bool operator<(const Light* other) const;
        Type GetType() const;
        // Params need to be transformed to view space
        virtual void ApplyToProgram(const Program *prog, const glm::mat4 &V) const = 0;

        glm::vec3 mIntensity;

        bool mIsActive;
        // For shadow rendering
        bool mHasChanged;

    private:
        Type mType;
    };

    //---------- AmbientLight ----------//
    struct AmbientLight : public Light
    {
        AmbientLight(const glm::vec3 &intensity, bool isActive);

        virtual void ApplyToProgram(const Program *prog, const glm::mat4 &V) const override;
    };


    //---------- DirectionalLight ----------//
    struct DirectionalLight : public Light
    {
        DirectionalLight(const glm::vec3 &intensity, bool isActive, const glm::vec3 &direction);

        virtual void ApplyToProgram(const Program *prog, const glm::mat4 &V) const override;

        void SetDirection(const glm::vec3 &direction);
        const glm::vec3& GetDirection() const;

    private:
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

        void SetPosition(const glm::vec3 &position);
        const glm::vec3& GetPosition() const;

        void SetDirection(const glm::vec3 &direction);
        const glm::vec3& GetDirection() const;

    private:
        //Constant, linear, quadratic
        glm::vec3 mAttenuation;
        //This is direction is the light is directional
        glm::vec3 mPosition;
        // SpotLight specific
        glm::vec3 mSpotDirection;
        float mSpotCutoff;
        float mSpotExponent;
    };
}

#endif
