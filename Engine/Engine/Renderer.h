
#ifndef EN_RENDERER_H
#define EN_RENDERER_H

#include <Engine/GL/TextureSampler.h>
#include <Engine/Utils/UniformBuffers.h>
#include <Engine/Geometry/AABB.h>
#include <vector>

namespace engine
{
    class Game;
    class Scene;
    class CameraHandler;
    class Camera;
    class Renderable;
    class RenderPass;
    struct Light;

    class Renderer
    {
    public:
        using RenderPasses = std::vector<RenderPass*>;

        Renderer(void);
        ~Renderer(void);

        void SetUsePBR(bool usePBR);
        void SetScene(Scene *scene);

        //Renderables
        void AddRenderable(Renderable *renderable);
        void RemoveRenderable(Renderable *renderable);

        void SetScreenSize(int width, int height);

        const RenderPass* GetRenderPass(const std::string &name) const;
        const RenderPass* GetRenderPass(int idx) const;
        const RenderPasses& GetRenderPasses() const;

        const Camera* GetCamera() const;
        bool UsePBR() const;
        AABB GetSceneAABB() const;

    private:
        Renderer(const Renderer &other);
        Renderer& operator=(const Renderer &other);

        RenderPass* GetRenderPass(const std::string &name);
        friend class Game;
        //Rendering
        void InitRendering(const CameraHandler *camera);
        void Render() const;

        const CameraHandler *mCamera;

        bool mUsePBR;

        std::vector<Renderable*> mRenderables;
        std::vector<RenderPass*> mRenderPasses;

        // Samplers
        TextureSampler mLinearMipMapSampler;
        TextureSampler mLinearSampler;
        TextureSampler mShadowmapSampler;
    };
}

#endif
