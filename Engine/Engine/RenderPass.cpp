
#include <Engine/Engine/RenderPass.h>
#include <Engine/Engine/Renderer.h>
#include <algorithm>

using namespace std;
using namespace glm;

namespace engine
{
    RenderPass::RenderPass(const string &name, GLbitfield clearMask, const vec4 &clearColor, bool clearBuffer)
        : mName(name), mClearMask(clearMask), mClearColor(clearColor), mClearBuffer(clearBuffer)
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
            glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a);
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
