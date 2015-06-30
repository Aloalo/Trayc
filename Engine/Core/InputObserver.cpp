/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Core/InputObserver.h>

namespace engine
{
    InputObserver::InputObserver(void) :
        mActive(true)
    {
    }

    InputObserver::~InputObserver(void)
    {
    }


    void InputObserver::HandleEvent(const SDL_Event &e)
    {

    }

    void InputObserver::KeyPress(const SDL_KeyboardEvent &e)
    {

    }

    void InputObserver::MouseMotion(const SDL_MouseMotionEvent &e)
    {

    }

    void InputObserver::MouseButton(const SDL_MouseButtonEvent &e)
    {

    }

    void InputObserver::MouseWheel(const SDL_MouseWheelEvent &e)
    {

    }

    void InputObserver::WindowEvent(const SDL_WindowEvent &e)
    {

    }

    void InputObserver::QuitEvent(const SDL_QuitEvent &e)
    {

    }
}
