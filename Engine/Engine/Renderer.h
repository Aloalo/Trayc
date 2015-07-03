/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_RENDERER_H
#define EN_RENDERER_H

#include <Engine/Engine/Renderable.h>
#include <Engine/Core/Camera.h>
#include <vector>
#include <map>

namespace engine
{
    enum ProgramType
    {
        PROGRAM_DIRECTIONAL_LIGHT = LIGHT_DIRECTIONAL,
        PROGRAM_POINT_LIGHT = LIGHT_POINT,
        PROGRAM_SPOT_LIGHT = LIGHT_SPOT,

        PROGRAM_DIRECTIONAL_SHADOW = LIGHT_DIRECTIONAL | LIGHT_CASTS_SHADOWS,
        PROGRAM_POINT_SHADOW = LIGHT_POINT | LIGHT_CASTS_SHADOWS,
        PROGRAM_SPOT_SHADOW = LIGHT_SPOT | LIGHT_CASTS_SHADOWS,

        PROGRAM_COUNT = 7
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
        Program mPrograms[PROGRAM_COUNT];
        std::vector<Renderable*> mRenderables;
    };
}

#endif
