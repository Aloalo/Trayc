
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

    class ShadowProjectionRenderPass : public RenderPass
    {
    public:
        ShadowProjectionRenderPass(void);

        virtual void Init() override;
        virtual void Destroy() override;

        virtual void Render(const RenderingContext &rContext) const override;

        // Inits shadow framebuffers
        void Init(const SceneGPUData *sceneData);

    private:
        TextureCombiner mShadowProjectionCombiners[Light::CT_LIGHT_TYPES];
        const SceneGPUData *mSceneData;
    };
}

#endif
