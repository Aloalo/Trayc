#ifndef ET_DEBUG_VIEW_H
#define ET_DEBUG_VIEW_H

#include <Engine/Engine/Renderable.h>
#include <Engine/Core/InputObserver.h>
#include <Engine/Engine/GlobalRenderingParams.h>

namespace engine
{
    class Rasterizer;
}

class DebugView 
    : public engine::Renderable, public engine::InputObserver
{
public:
    DebugView(const engine::Rasterizer *renderer, float nearDist, float farDist);

    virtual void KeyPress(const SDL_KeyboardEvent &e) override;
    virtual void Draw(const engine::RenderingContext &rContext) const override;
private:
    const engine::Rasterizer *mRenderer;
    engine::TextureType mTexType;
    float mNearDist, mFarDist;
    bool mDrawGloss;

};
#endif
