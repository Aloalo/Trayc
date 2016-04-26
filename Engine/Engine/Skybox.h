
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
        void Destroy();

        void Draw(const Camera *camera) const;

    private:
        Program mSkyboxProg;
        CubemapTexture mSkyboxCubemap;
        VertexArray mSkyboxVA;
        float mFarPlaneMod;
    };
}

#endif
