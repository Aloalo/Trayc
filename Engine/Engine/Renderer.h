/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_RENDERER_H
#define EN_RENDERER_H

#include <Engine/Engine/Renderable.h>
#include <Engine/Core/Camera.h>
#include <vector>

namespace engine
{
    enum ProgramType
    {
        PROGRAM_NONE = 0,
        PROGRAM_DIRECTIONAL_LIGHT = LightFlag::LIGHT_DIRECTIONAL,
        PROGRAM_POINT_LIGHT = LightFlag::LIGHT_POINT,
        PROGRAM_SPOT_LIGHT = LightFlag::LIGHT_SPOT,

        PROGRAM_SHADOW_MAP = LightFlag::LIGHT_CASTS_SHADOWS,
        PROGRAM_INSANCED = 1 << 4,

        PROGRAM_COUNT = 1 << 5
    };

    class Renderer
    {
    public:
        Renderer(void);
        ~Renderer(void);

        void SetClearColor(const glm::vec4 &clearColor) const;

        //Renderables
        void AddRenderable(Renderable *renderable);
        void RemoveRenderable(Renderable *renderable);

        //Lights
        void AddLight(const Light &light);
        void RemoveLight(int idx);
        const std::vector<Light>& GetLights() const;
        const Light& GetLight(int idx) const;
        Light& GetLight(int idx);

    private:
        friend class Scene;
        //Rendering
        void InitRendering(const Camera *camera);
        void Render() const;

        const Camera *mCamera;
        GLbitfield mClearMask;
        std::vector<Light> mLights;
        std::vector<Renderable*> mRenderables;
    };
}

#endif
