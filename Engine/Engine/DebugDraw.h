#ifndef EN_DEBUG_DRAW_H
#define EN_DEBUG_DRAW_H

#include <Engine/Engine/TextureCombiner.h>
#include <Engine/Utils/Singleton.h>

namespace engine
{
    // Combines multiple textures into one using a program
    class DebugDraw : public Singleton<DebugDraw>
    {
    public:
        void DrawDepth(const Texture2D &tex, float nearRange, float farRange) const;
        // Draws rgb part of the texture on screen
        void DrawTexture(const Texture2D &tex, const glm::vec3 &gamma) const;
        void DrawNormal(const Texture2D &tex) const;
        void DrawGloss(const Texture2D &tex) const;

        void Destroy();

    private:
        friend engine::Singleton<DebugDraw>;

        DebugDraw(void);

        TextureCombiner mDrawDepth;
        TextureCombiner mDrawTex;
        TextureCombiner mDrawNormals;
        TextureCombiner mDrawGloss;
    };
}

#endif
