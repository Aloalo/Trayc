#ifndef RTRT_GUI_VIEW_H
#define RTRT_GUI_VIEW_H

#include <Engine/Engine/Renderable.h>
#include <Engine/Core/InputObserver.h>
#include <Engine/GL/VertexArray.h>
#include <Engine/GL/Program.h>

namespace engine
{
    class RayTracedGame;
}

class GUIView 
    : public engine::Renderable, public engine::InputObserver
{
public:
    explicit GUIView(engine::RayTracedGame *game);
    ~GUIView(void);

    virtual void KeyPress(const SDL_KeyboardEvent &e) override;
    virtual void Draw(const engine::RenderingContext &rContext) const override;

    void DisableInput();
private:
    engine::RayTracedGame *mGame;
    engine::VertexArray mVA;
    engine::Program mProgram;
};

#endif
