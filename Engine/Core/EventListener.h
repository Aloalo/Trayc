/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_EVENTLISTENER_H
#define EN_EVENTLISTENER_H

#include "SDL.h"

namespace engine
{
    class EventListener
    {
    public:
        EventListener(void);
        virtual ~EventListener(void);

        virtual void HandleEvent(const SDL_Event &e);

        bool active;
    };
}

#endif
