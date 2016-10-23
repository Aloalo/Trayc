
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

        // Sets shadowmaps to lights
        void Init(const SceneGPUData *sceneData);

        const Texture* GetShadowmap(const Light *light) const;
        glm::mat4 GetDepthBiasVP(const glm::vec3 &viewDir) const;

    private:
        Program mProgram;
        const SceneGPUData *mSceneData;
        std::map<const Light*, const Texture*> mLightToShadowmap;
    };
}

#endif
