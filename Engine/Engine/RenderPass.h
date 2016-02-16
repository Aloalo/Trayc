
#ifndef EN_RENDER_PASS_H
#define EN_RENDER_PASS_H

#include <Engine/GL/FrameBuffer.h>
#include <Engine/Engine/RenderingContext.h>

namespace engine
{
    // Represents a render pass
    // e.g. geometry pass, lighting pass ...
    class RenderPass
    {
    public:
        RenderPass(const std::string &name, GLbitfield clearMask);
        virtual ~RenderPass(void);

        // renderPasses are so far initialized render passes
        virtual void Init() = 0;
        virtual void Destroy() = 0;

        void BeginRender() const;

        // RenderPass extracts all data it needs to render from renderPasses and rContext
        virtual void Render(const RenderingContext &rContext) const = 0;

        const std::string& GetName() const;
        const FrameBuffer& GetDstBuffer() const;

        void ResizeDstBuffer(int width, int height);

    protected:
        static const RenderPass* GetRenderPass(const std::vector<RenderPass*> &renderPasses, const std::string &name);
        FrameBuffer mDstFB;

    private:
        std::string mName;
        GLbitfield mClearMask;
    };
}

#endif
