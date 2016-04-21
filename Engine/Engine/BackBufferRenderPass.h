
#ifndef EN_BACK_BUFFER_RENDER_PASS_H
#define EN_BACK_BUFFER_RENDER_PASS_H

#include <Engine/Engine/RenderPass.h>
#include <Engine/Engine/TextureCombiner.h>

namespace engine
{
    // Final render pass to backbuffer
    class BackBufferRenderPass : public RenderPass
    {
    public:
        BackBufferRenderPass(void);

        virtual void Init() override;
        virtual void Destroy() override;

        virtual void Render(const RenderingContext &rContext) const override;
    private:
        const FrameBuffer *mFinalFB;
    };
}

#endif
