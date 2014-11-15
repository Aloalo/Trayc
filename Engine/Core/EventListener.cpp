/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Core/EventListener.h>

namespace engine
{
    EventListener::EventListener(void) :
        active(true)
    {
    }

    EventListener::~EventListener(void)
    {
    }

    void EventListener::HandleEvent(const SDL_Event &e)
    {
    }
}
