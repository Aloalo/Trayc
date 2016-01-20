/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef MB_MANDELBROT_RENDERER_H
#define MB_MANDELBROT_RENDERER_H

#include <Engine/Engine/Renderable.h>
#include <Engine/Core/InputObserver.h>
#include <Engine/GL/VertexArray.h>
#include <Engine/GL/Program.h>

class MandelbrotRenderer : public engine::Renderable, public engine::InputObserver
{
public:
    MandelbrotRenderer(glm::vec2 windowSize);
    ~MandelbrotRenderer(void);

    virtual void KeyPress(const SDL_KeyboardEvent &e) override;
    virtual void MouseMotion(const SDL_MouseMotionEvent &e) override;
    virtual void MouseButton(const SDL_MouseButtonEvent &e) override;
    virtual void MouseWheel(const SDL_MouseWheelEvent &e) override;
    virtual void WindowEvent(const SDL_WindowEvent &e) override;

    virtual void Draw(const engine::RenderingContext &rContext) const override;

private:
    void ApplyState() const;

    engine::VertexArray mQuad;
    engine::Program mMandelProg;

    bool mMovingView;

    struct State
    {
        glm::vec2 mTopLeftCoord;
        glm::vec2 mWindowSize;
        float mScale;
        int mLimit;
        int mAA;
    } mState;

};

#endif
