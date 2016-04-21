
#include <Engine/Engine/BackBufferRenderPass.h>
#include <Engine/Engine/Renderer.h>

namespace engine
{
    BackBufferRenderPass::BackBufferRenderPass(void)
        : RenderPass("bbPass", GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT), mFinalFB(nullptr)
    {
    }

    void BackBufferRenderPass::Init()
    {
        const auto& renderPasses = mRenderer->GetRenderPasses();
        // BackBufferRenderPass is actually the last pass 
        const RenderPass* lastPass = renderPasses[renderPasses.size() - 2];
        mFinalFB = &lastPass->GetDstBuffer();
    }

    void BackBufferRenderPass::Destroy()
    {
    }

    void BackBufferRenderPass::Render(const RenderingContext &rContext) const
    {
        mFinalFB->BindRead();
        FrameBuffer::BackBuffer().BindDraw();
        glBlitFramebuffer(0, 0, mFinalFB->Width(), mFinalFB->Height(), 0, 0, mDstFB.Width(), mDstFB.Height(), GL_COLOR_BUFFER_BIT, GL_LINEAR);
        FrameBuffer::UnBind();
    }

}
