
#ifndef EN_SHADOW_RENDER_PASS_H
#define EN_SHADOW_RENDER_PASS_H

#include <Engine/Engine/RenderPass.h>
#include <Engine/Engine/Light.h>
#include <Engine/GL/Program.h>
#include <Engine/GL/VertexArray.h>
#include <Engine/Engine/GlobalRenderingParams.h>

namespace engine
{
    struct SceneGPUData;
    class scene;

    __declspec(align(16)) class ShadowRenderPass : public RenderPass
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

        ShadowRenderPass(void);

        virtual void Init() override;
        virtual void Destroy() override;

        virtual void Render(const RenderingContext &rContext) const override;

        // Inits shadow framebuffers
        void Init(const SceneGPUData *sceneData);
        const Texture2D& GetShadowmap(int idx) const;

    private:
        Program mShadowMappingProgram;
        const SceneGPUData *mSceneData;
        std::vector<FrameBuffer> mShadowFBs;
    };
}

#endif
