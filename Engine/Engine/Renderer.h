/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_RENDERER_H
#define EN_RENDERER_H

#include <Engine/Engine/Renderable.h>
#include <Engine/Engine/RenderPass.h>

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
        Renderer(Game *scene);
        ~Renderer(void);

        void SetScene(const Scene *scene);

        //Renderables
        void AddRenderable(Renderable *renderable);
        void RemoveRenderable(Renderable *renderable);

        void AddLight(const Light *light);
        void RemoveLight(const Light *light);

        void SetScreenSize(int width, int height);

        const RenderPass* GetRenderPass(const std::string &name) const;

    private:
        RenderPass* GetRenderPass(const std::string &name);
        friend class Game;
        //Rendering
        void InitRendering(const CameraHandler *camera);
        void Render() const;

        const CameraHandler *mCamera;

        std::vector<Renderable*> mRenderables;
        std::vector<RenderPass*> mRenderPasses;
    };
}

#endif
