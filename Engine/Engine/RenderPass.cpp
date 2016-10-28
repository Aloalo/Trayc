
#include <Engine/Engine/RenderPass.h>
#include <Engine/Engine/Renderer.h>
#include <algorithm>

using namespace std;

namespace engine
{
    RenderPass::RenderPass(const string &name, GLbitfield clearMask, bool clearBuffer)
        : mName(name), mClearMask(clearMask), mClearBuffer(clearBuffer)
    {
    }

    RenderPass::~RenderPass(void)
    {
    }

    void RenderPass::BeginRender() const
    {
        mDstFB.Bind();
        glViewport(0, 0, mDstFB.Width(), mDstFB.Height());
        if(mClearBuffer) {
            glClear(mClearMask);
        }
    }

    void RenderPass::SetRenderer(Renderer *renderer)
    {
        mRenderer = renderer;
    }

    const string& RenderPass::GetName() const
    {
        return mName;
    }

    const FrameBuffer& RenderPass::GetDstBuffer() const
    {
        return mDstFB;
    }

    void RenderPass::ResizeDstBuffer(int width, int height)
    {
        mDstFB.Resize(width, height);
    }
}
