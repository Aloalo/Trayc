
#ifndef EN_BACK_BUFFER_RENDER_PASS_H
#define EN_BACK_BUFFER_RENDER_PASS_H

#include <Engine/Engine/RenderPass.h>
#include <Engine/Engine/TextureCombiner.h>

namespace engine
{
    // Final render pass to backbuffer
    __declspec(align(16)) class BackBufferRenderPass : public RenderPass
    {
    public:
        void* operator new(size_t i)
        {
            return _mm_malloc(i, 16);
        }

        void operator delete(void* p)
        {
            _mm_free(p);
        }

        BackBufferRenderPass(void);

        virtual void Init() override;
        virtual void Destroy() override;

        void CompileShaders();

        virtual void Render(const RenderingContext &rContext) const override;
    private:
        const Texture2D *mFinalTex;
        TextureCombiner mDraw;
    };
}

#endif
