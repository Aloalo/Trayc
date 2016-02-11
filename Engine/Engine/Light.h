
#ifndef EN_LIGHT_H
#define EN_LIGHT_H

#include <Engine/GL/Program.h>

namespace engine
{
    enum LightType
    {
        DIRECTIONAL = 0,
        POINT,
        SPOT,
    };

    struct Light
    {
        Light(const glm::vec3 &intensity, LightType type, bool isActive);
        virtual ~Light();

        // Call this from derived functions
        virtual void ApplyToProgram(const Program &prog) const = 0;

        bool operator<(const Light* other) const;

        glm::vec3 mIntensity;
        LightType mType;
        bool mIsActive;
    };

    struct DirectionalLight : public Light
    {
        DirectionalLight(const glm::vec3 &intensity, bool isActive);

        virtual void ApplyToProgram(const Program &prog) const override;
    };

    struct PointLight : public Light
    {
        PointLight(const glm::vec3 &intensity, 
            bool isActive,
            const glm::vec3 &attenuation,
            const glm::vec3 &position);

        virtual void ApplyToProgram(const Program &prog) const override;

        //Constant, linear, quadratic
        glm::vec3 mAttenuation;
        //This is direction is the light is directional
        glm::vec3 mPosition;
    };

    struct SpotLight : public Light
    {
        SpotLight(const glm::vec3 &intensity,
            bool isActive,
            const glm::vec3 &attenuation,
            const glm::vec3 &position,
            const glm::vec3 &spotDir,
            float spotCutoff,
            float spotExp);

        virtual void ApplyToProgram(const Program &prog) const override;

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
