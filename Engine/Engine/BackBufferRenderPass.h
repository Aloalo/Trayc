
#ifndef EN_BACK_BUFFER_RENDER_PASS_H
#define EN_BACK_BUFFER_RENDER_PASS_H

#include <Engine/Engine/RenderPass.h>
#include <Engine/Engine/TextureCombiner.h>

namespace engine
{
    // Final render pass to backbuffer
    class BackBufferRenderPass : public RenderPass
    {
    public:
        BackBufferRenderPass(void);
        ~BackBufferRenderPass(void);

        virtual void Init() override;
        virtual void Destroy() override;

        void SetFxaa(bool flag);
        void ToggleFxaa();

        void CompileShaders();

        virtual void Render(const RenderingContext &rContext) const override;
        virtual void ResizeDstBuffer(int width, int height) override;
    private:
        const Texture2D *mFinalTex;
        Texture2D mNoiseTex;
        TextureCombiner mDraw;

        bool mFxaa;
        FrameBuffer mLumasRGBFB;
        TextureCombiner mFxaaCombiner;
    };
}

#endif
