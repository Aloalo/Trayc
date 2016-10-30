#ifndef EN_EVENTLISTENER_H
#define EN_EVENTLISTENER_H

#include "SDL.h"

namespace engine
{
    class InputObserver
    {
    public:
        InputObserver(void);
        virtual ~InputObserver(void);

        virtual void HandleEvent(const SDL_Event &e);
        virtual void KeyPress(const SDL_KeyboardEvent &e);
        virtual void MouseMotion(const SDL_MouseMotionEvent &e);
        virtual void MouseButton(const SDL_MouseButtonEvent &e);
        virtual void MouseWheel(const SDL_MouseWheelEvent &e);
        virtual void WindowEvent(const SDL_WindowEvent &e);
        virtual void QuitEvent(const SDL_QuitEvent &e);

        //Is it active
        bool mActive;
    };
}

#endif
