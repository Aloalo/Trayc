
#ifndef EN_GEOMETRY_RENDER_PASS_H
#define EN_GEOMETRY_RENDER_PASS_H

#include <Engine/Engine/RenderPass.h>
#include <Engine/GL/Program.h>
#include <Engine/GL/VertexArray.h>
#include <Engine/Engine/GlobalRenderingParams.h>

namespace engine
{
    class Scene;

    struct SceneGPUData
    {
        SceneGPUData(void);

        void Init(const Scene *scene);

        void Destroy();
        void ClearVertexArrays();

        std::map<std::string, Texture2D> mNameToTex;
        // At index i is VA for mesh i
        std::vector<VertexArray> mVertexArrays;
        const Scene *mScene;
    };

    class GeometryRenderPass : public RenderPass
    {
    public:
        GeometryRenderPass(void);

        virtual void Init() override;
        virtual void Destroy() override;

        virtual void Render(const RenderingContext &rContext) const override;

        void Init(const Scene *scene);
        const SceneGPUData* GetGPUSceneData() const;

    private:
        Program mGPrograms[1 << TextureType::CT_MAT_TEXTURE_TYPES];
        SceneGPUData mSceneData;
    };
}

#endif
