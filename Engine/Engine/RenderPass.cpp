
#include <Engine/Engine/RenderPass.h>
#include <algorithm>

using namespace std;

namespace engine
{
    RenderPass::RenderPass(const std::string &name, GLbitfield clearMask)
        : mName(name), mClearMask(clearMask)
    {
    }

    RenderPass::~RenderPass(void)
    {
    }

    void RenderPass::BeginRender() const
    {
        mDstFB.Bind();
        glViewport(0, 0, mDstFB.Width(), mDstFB.Height());
        glClear(mClearMask);
    }

    const std::string & RenderPass::GetName() const
    {
        return mName;
    }

    const FrameBuffer & RenderPass::GetDstBuffer() const
    {
        return mDstFB;
    }

    void RenderPass::ResizeDstBuffer(int width, int height)
    {
        mDstFB.Resize(width, height);
    }

    const RenderPass *RenderPass::GetRenderPass(const vector<RenderPass*>& renderPasses, const string &name)
    {
        return *find_if(renderPasses.begin(), renderPasses.end(), [&](const RenderPass *rPass)
        {
            return rPass->GetName() == name;
        });;
    }
}
