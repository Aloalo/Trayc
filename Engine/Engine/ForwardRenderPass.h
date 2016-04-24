
#ifndef EN_FORWARD_RENDER_PASS_H
#define EN_FORWARD_RENDER_PASS_H

#include <Engine/Engine/RenderPass.h>
#include <Engine/GL/Program.h>

namespace engine
{
    class Scene;

    class ForwardRenderPass : public RenderPass
    {
    public:
        ForwardRenderPass(void);

        virtual void Init() override;
        virtual void Destroy() override;

        virtual void BeginRender() const override;

        virtual void Render(const RenderingContext &rContext) const override;

        virtual const FrameBuffer& GetDstBuffer() const override;

    private:
        Program mForwardProg;
    };
}

#endif
