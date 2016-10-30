
#ifndef EN_SHADOW_RENDER_PASS_H
#define EN_SHADOW_RENDER_PASS_H

#include <Engine/Engine/RenderPass.h>
#include <Engine/GL/Program.h>
#include <Engine/GL/VertexArray.h>
#include <Engine/Engine/GlobalRenderingParams.h>

namespace engine
{
    struct SceneGPUData;
    class scene;

    class ShadowRenderPass : public RenderPass
    {
    public:
        ShadowRenderPass(void);

        virtual void Init() override;
        virtual void Destroy() override;

        virtual void Render(const RenderingContext &rContext) const override;

        // Inits shadow framebuffers
        void Init(const SceneGPUData *sceneData);
        const Texture2D& GetShadowmap(int idx) const;

    private:
        Program mProgram;
        const SceneGPUData *mSceneData;
        std::vector<FrameBuffer> mShadowFBs;
    };
}

#endif
