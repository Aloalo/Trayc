
#ifndef EN_FORWARD_RENDER_PASS_H
#define EN_FORWARD_RENDER_PASS_H

#include <Engine/Engine/RenderPass.h>
#include <Engine/Engine/Skybox.h>
#include <Engine/GL/Program.h>
#include <Engine/GL/CubemapTexture.h>
#include <Engine/GL/VertexArray.h>

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

        const glm::mat4& GetSkyboxM() const;

    private:
        Program mForwardProg;
        Skybox mSkybox;
    };
}

#endif
