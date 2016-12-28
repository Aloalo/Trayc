#ifndef ET_GUI_VIEW_H
#define ET_GUI_VIEW_H

#include <Engine/Engine/Renderable.h>
#include <Engine/Core/InputObserver.h>
#include <Engine/GL/VertexArray.h>
#include <Engine/GL/Program.h>

namespace engine
{
    class Game;
}

class GUIView 
    : public engine::Renderable, public engine::InputObserver
{
public:
    explicit GUIView(engine::Game *game);
    ~GUIView(void);

    virtual void KeyPress(const SDL_KeyboardEvent &e) override;
    virtual void Draw(const engine::RenderingContext &rContext) const override;
private:
    engine::Game *mGame;
    engine::VertexArray mVA;
    engine::Program mProgram;
};
#endif
