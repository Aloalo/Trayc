/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_RENDERER_H
#define EN_RENDERER_H

#include <Engine/Engine/Renderable.h>
#include <Engine/Core/CameraHandler.h>
#include <Engine/GL/FrameBuffer.h>
#include <vector>

namespace engine
{
    class Scene;

    class Renderer
    {
    public:
        Renderer(Scene *scene);
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
        void InitRendering(const CameraHandler *camera);
        void Render() const;

        const CameraHandler *mCamera;
        Scene *mScene;

        engine::FrameBuffer mGBuffer;
        GLbitfield mClearMask;
        std::vector<Light> mLights;
        std::vector<Renderable*> mRenderables;
    };
}

#endif
