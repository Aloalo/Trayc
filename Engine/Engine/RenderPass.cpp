
#include <Engine/Engine/RenderPass.h>

using namespace std;

namespace engine
{
    RenderPass::RenderPass(const std::string &name)
        : mName(name)
    {
    }

    RenderPass::~RenderPass(void)
    {
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
        mDstFB.Bind();
        glViewport(0, 0, width, height);
        FrameBuffer::UnBind();
    }
}
