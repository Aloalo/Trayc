
#ifndef EN_SHADOW_PROJECTION_RENDER_PASS_H
#define EN_SHADOW_PROJECTION_RENDER_PASS_H

#include <Engine/Engine/RenderPass.h>
#include <Engine/Engine/TextureCombiner.h>
#include <Engine/Engine/Light.h>
#include <Engine/GL/VertexArray.h>
#include <Engine/Engine/GlobalRenderingParams.h>

namespace engine
{
    struct SceneGPUData;
    class scene;

    __declspec(align(16)) class ShadowProjectionRenderPass : public RenderPass
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

        ShadowProjectionRenderPass(void);

        virtual void Init() override;
        virtual void Destroy() override;

        virtual void Render(const RenderingContext &rContext) const override;

        const Texture2D& GetProjectedShadowmap(int idx) const;

        // Inits shadow framebuffers
        void Init(const SceneGPUData *sceneData);

    private:
        TextureCombiner mShadowProjectionCombiners[Light::CT_LIGHT_TYPES];
        const SceneGPUData *mSceneData;
        std::vector<FrameBuffer> mProjectedShadowFBs;
    };
}

#endif
