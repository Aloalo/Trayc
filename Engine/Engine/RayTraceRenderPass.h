
#ifndef EN_RAY_TRACE_RENDER_PASS_H
#define EN_RAY_TRACE_RENDER_PASS_H

#include <Engine/Engine/RenderPass.h>
#include <Engine/Engine/Light.h>
#include <Engine/Engine/TextureCombiner.h>

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
    private:
        TextureCombiner mRayTraceCombiner;
    };
}

#endif
