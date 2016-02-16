
#ifndef EN_GEOMETRY_RENDER_PASS_H
#define EN_GEOMETRY_RENDER_PASS_H

#include <Engine/Engine/RenderPass.h>
#include <Engine/GL/Program.h>
#include <Engine/GL/VertexArray.h>
#include <Engine/Engine/GlobalRenderingParams.h>

namespace engine
{
    class Scene;

    class GeometryRenderPass : public RenderPass
    {
    public:
        GeometryRenderPass(void);

        virtual void Init() override;
        virtual void Destroy() override;

        virtual void Render(const RenderingContext &rContext) const override;

        void InitScene(const Scene *scene);

    private:
        void ClearVertexArrays();

        const Scene *mScene;

        Program mGPrograms[1 << TextureType::CT_MAT_TEXTURE_TYPES];
        std::map<std::string, Texture2D> mNameToTex;
        // At index i is VA for mesh i
        std::vector<VertexArray> mVertexArrays;
    };
}

#endif
