
#ifndef EN_FORWARD_RENDER_PASS_H
#define EN_FORWARD_RENDER_PASS_H

#include <Engine/Engine/RenderPass.h>

namespace engine
{
    class Scene;

    class ForwardRenderPass : public RenderPass
    {
    public:
        ForwardRenderPass(void);

        virtual void Init() override;
        virtual void Destroy() override;

        virtual void Render(const RenderingContext &rContext) const override;

    private:
        const RenderPass *gPass;
        const RenderPass *lPass;
    };
}

#endif
