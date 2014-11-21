/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Core/EventHandler.h>
#include <Engine/Core/SDLHandler.h>
#include <AntTweakBar.h>
#include "SDL.h"

using namespace std;

namespace engine
{
    vector<EventListener*> EventHandler::listenerList;
    vector<Updateable*> EventHandler::updateableList;
    bool EventHandler::quit = false;
    bool EventHandler::isCursorFree = true;
    float EventHandler::timeStep = 1.0f / 60.0f;
    float EventHandler::accumulator = 0.0f;

    void EventHandler::ProcessPolledEvents()
    {
        SDL_Event test_event;
        while(SDL_PollEvent(&test_event))
        {
            if(test_event.type == SDL_QUIT)
                quit = true;
            else if(test_event.type == SDL_KEYDOWN && test_event.key.keysym.sym == SDLK_LSHIFT)
            {
                if(isCursorFree)
                {
                    SDL_SetRelativeMouseMode(SDL_FALSE);
                    SDL_ShowCursor(1);
                }
                else
                {
                    SDL_SetRelativeMouseMode(SDL_TRUE);
                    SDL_ShowCursor(0);
                }
                isCursorFree = !isCursorFree;
            }
            if(isCursorFree == false)
            {
                TwEventSDL(&test_event, SDLHandler::linked.major, SDLHandler::linked.minor);
                if(test_event.type == SDL_MOUSEMOTION)
                    continue;
            }

            if(quit == true)
                test_event.type = SDL_QUIT;

            for(EventListener *listener : listenerList)
                if(listener->active)
                    listener->HandleEvent(test_event);

            if(quit == true)
                break;
        }
    }

    void EventHandler::AddEventListener(EventListener *listener)
    {
        listenerList.push_back(listener);
    }

    void EventHandler::RemoveEventListener(const EventListener *listener)
    {
        //TODO_JURE finish
    }

    bool EventHandler::Quit()
    {
        return quit;
    }

    void EventHandler::Update()
    {
        static float lastTime = static_cast<float>(SDL_GetTicks()) / 1000.0f;
        const float currentTime = static_cast<float>(SDL_GetTicks()) / 1000.0f;
        accumulator += currentTime - lastTime;
        lastTime = currentTime;

        for(; accumulator > timeStep; accumulator -= timeStep)
            for(Updateable *updateable : updateableList)
                if(updateable->active)
                    updateable->Update(timeStep);
    }

    void EventHandler::AddUpdateable(Updateable *updateable)
    {
        updateableList.push_back(updateable);
    }

    void EventHandler::RemoveUpdateable(const Updateable *updateable)
    {
        //TODO_JURE finish
    }

    void EventHandler::SetQuit()
    {
        quit = true;
    }

}
