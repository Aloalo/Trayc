/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_RENDERER_H
#define EN_RENDERER_H

#include <Engine/Engine/Renderable.h>
#include <Engine/Engine/RenderPass.h>
#include <Engine/GL/TextureSampler.h>
#include <Engine/Utils/ViewRayDataUB.h>

namespace engine
{
    class Game;
    class Scene;
    class CameraHandler;
    class Renderable;
    class RenderPass;
    struct Light;

    class Renderer
    {
    public:
        using RenderPasses = std::vector<RenderPass*>;

        Renderer();
        ~Renderer(void);

        void SetScene(const Scene *scene);

        //Renderables
        void AddRenderable(Renderable *renderable);
        void RemoveRenderable(Renderable *renderable);

        void SetScreenSize(int width, int height);

        const RenderPass* GetRenderPass(const std::string &name) const;
        const RenderPass* GetRenderPass(int idx) const;
        const RenderPasses& GetRenderPasses() const;

    private:
        RenderPass* GetRenderPass(const std::string &name);
        friend class Game;
        //Rendering
        void InitRendering(const CameraHandler *camera);
        void Render() const;

        const CameraHandler *mCamera;

        std::vector<Renderable*> mRenderables;
        std::vector<RenderPass*> mRenderPasses;

        TextureSampler mLinearMipMapSampler;
        TextureSampler mLinearSampler;
        ViewRayDataUB mViewRayDataUB;
    };
}

#endif
