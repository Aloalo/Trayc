
#ifndef EN_SKYBOX_H
#define EN_SKYBOX_H

#include <Engine/GL/Program.h>
#include <Engine/GL/CubemapTexture.h>
#include <Engine/GL/VertexArray.h>
#include <Engine/Engine/Renderable.h>

namespace engine
{
    class Renderer;
    class Camera;

    class Skybox
    {
    public:
        Skybox(void);

        void Init(const Renderer *renderer);
        void LoadSkybox(int skyboxIdx);
        void Destroy();

        void Draw(const Camera *camera) const;
        const glm::mat4& GetTransform() const;

    private:
        Program mSkyboxProg;
        std::vector<CubemapTexture> mSkyboxCubemaps;
        std::vector<CubemapTexture> mIrradianceMaps;
        VertexArray mSkyboxVA;
        glm::mat4 mSkyboxTransform;
        glm::mat4 mSkyboxScale;
        float mFarPlaneMod;
    };
}

#endif
