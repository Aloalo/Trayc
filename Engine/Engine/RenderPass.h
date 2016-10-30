
#ifndef EN_RENDER_PASS_H
#define EN_RENDER_PASS_H

#include <Engine/GL/FrameBuffer.h>
#include <Engine/Engine/RenderingContext.h>

namespace engine
{
    class Renderer;

    // Represents a render pass
    // e.g. geometry pass, lighting pass ...
    class RenderPass
    {
    public:
        RenderPass(const std::string &name, GLbitfield clearMask, bool clearBuffer = true);
        virtual ~RenderPass(void);

        // renderPasses are so far initialized render passes
        virtual void Init() = 0;
        virtual void Destroy() = 0;

        virtual void BeginRender() const;

        // RenderPass extracts all data it needs to render from renderPasses and rContext
        virtual void Render(const RenderingContext &rContext) const = 0;

        void SetRenderer(Renderer *renderer);

        const std::string& GetName() const;
        virtual const FrameBuffer& GetDstBuffer() const;

        void ResizeDstBuffer(int width, int height);

    protected:
        const Renderer *mRenderer;
        FrameBuffer mDstFB;
        GLbitfield mClearMask;
        // Should the buffer be cleard in BeginRender()
        bool mClearBuffer;

    private:
        std::string mName;
    };
}

#endif
