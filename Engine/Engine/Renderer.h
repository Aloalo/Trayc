
#ifndef EN_RENDERER_H
#define EN_RENDERER_H

#include <Engine/GL/TextureSampler.h>
#include <Engine/Utils/UniformBuffers.h>
#include <Engine/Geometry/AABB.h>
#include <Engine/Geometry/RayTracePrimitives.h>
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
        virtual ~Renderer(void);

        //Renderables
        void AddRenderable(Renderable *renderable);
        void RemoveRenderable(Renderable *renderable);

        virtual void SetScreenSize(int width, int height) = 0;

        const RenderPass* GetRenderPass(const std::string &name) const;
        RenderPass* GetRenderPass(const std::string &name);
        const RenderPass* GetRenderPass(int idx) const;
        const RenderPasses& GetRenderPasses() const;

        const Camera* GetCamera() const;
        AABB GetSceneAABB() const;
        GLubyte* TakeScreenshot(int &width, int &height) const;

    protected:
        void SetViewRayData();
        
        friend class Game;
        //Rendering
        virtual void InitRendering(const CameraHandler *camera) = 0;
        virtual void Render() const = 0;

        const CameraHandler *mCamera;

        std::vector<Renderable*> mRenderables;
        std::vector<RenderPass*> mRenderPasses;
    };

    class Rasterizer : public Renderer
    {
    public:
        Rasterizer(void);
        ~Rasterizer(void);

        void SetUsePBR(bool usePBR);
        bool UsePBR() const;
        void SetScene(Scene *scene);
        virtual void SetScreenSize(int width, int height) override;
        void LoadSkybox(int idx);

    private:
        bool mUsePBR;

        // Samplers
        TextureSampler mLinearMipMapSampler;
        TextureSampler mLinearSampler;
        TextureSampler mShadowmapSampler;

        virtual void InitRendering(const CameraHandler *camera) override;
        virtual void Render() const override;
    };

    class RayTraceRenderPass;

    class RayTracer : public Renderer
    {
    public:
        RayTracer(void);
        ~RayTracer(void);

        void AddSphere(const RTSphere &sphere);
        void AddLight(const RTLight &light);
        void AddRectangle(const RTRectangle &rect);
        void AddBox(const RTBox &rect);

        virtual void SetScreenSize(int width, int height) override;
    private:
        virtual void InitRendering(const CameraHandler *camera) override;
        virtual void Render() const override;

        RayTraceRenderPass *mRTPass;
    };
}

#endif
