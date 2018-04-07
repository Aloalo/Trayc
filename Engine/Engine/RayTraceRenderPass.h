
#ifndef EN_RAY_TRACE_RENDER_PASS_H
#define EN_RAY_TRACE_RENDER_PASS_H

#include <Engine/Engine/RenderPass.h>
#include <Engine/Engine/TextureCombiner.h>
#include <Engine/Geometry/RayTracePrimitives.h>

namespace engine
{
    class RayTraceRenderPass : public RenderPass
    {
    public:
        RayTraceRenderPass(void);

        virtual void Init() override;
        virtual void Destroy() override;

        virtual void Render(const RenderingContext &rContext) const override;

        void CompileShaders();

        void AddSphere(const RTSphere &sphere);
        void AddLight(const RTLight &light);
    private:
        void UploadToGPU() const;

        TextureCombiner mRayTraceCombiner;
        std::vector<RTSphere> mSpheres;
        std::vector<RTLight> mLights;
    };
}

#endif
