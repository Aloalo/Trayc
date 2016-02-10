#ifndef EN_DEBUG_DRAW_H
#define EN_DEBUG_DRAW_H

#include <Engine/Engine/Renderable.h>
#include <Engine/Core/InputObserver.h>

namespace engine
{
    class Game;
}

class DebugView 
    : public engine::Renderable, public engine::InputObserver
{
public:
    DebugView(engine::Game *game);

    virtual void KeyPress(const SDL_KeyboardEvent &e) override;
    virtual void Draw(const engine::RenderingContext &rContext) const override;
private:
    engine::Game *mGame;

};
#endif
