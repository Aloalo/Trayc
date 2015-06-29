/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Core/EventHandler.h>
#include <Engine/Core/SDLHandler.h>
#include "SDL.h"

using namespace std;

namespace engine
{
    vector<EventListener*> EventHandler::mListenerList;
    vector<Updateable*> EventHandler::mUpdateableList;
    bool EventHandler::mQuit = false;
    bool EventHandler::mIsCursorFree = true;
    float EventHandler::mTimeStep = 1.0f / 60.0f;
    float EventHandler::mAccumulator = 0.0f;

    void EventHandler::ProcessPolledEvents()
    {
        SDL_Event test_event;
        while(SDL_PollEvent(&test_event))
        {
            if(test_event.type == SDL_QUIT)
                mQuit = true;

            else if(test_event.type == SDL_KEYDOWN && test_event.key.keysym.sym == SDLK_LSHIFT)
            {
                if(mIsCursorFree)
                {
                    SDL_SetRelativeMouseMode(SDL_FALSE);
                    SDL_ShowCursor(1);
                }
                else
                {
                    SDL_SetRelativeMouseMode(SDL_TRUE);
                    SDL_ShowCursor(0);
                }
                mIsCursorFree = !mIsCursorFree;
            }

            for(EventListener *listener : mListenerList)
                if(listener->mActive)
                    listener->HandleEvent(test_event);

            if(mQuit == true)
                break;
        }
    }

    void EventHandler::AddEventListener(EventListener *listener)
    {
        mListenerList.push_back(listener);
    }

    void EventHandler::RemoveEventListener(const EventListener *listener)
    {
        mListenerList.erase(find(mListenerList.begin(), mListenerList.end(), listener));
    }

    bool EventHandler::Quit()
    {
        return mQuit;
    }

    void EventHandler::Update()
    {
        static float lastTime = static_cast<float>(SDL_GetTicks()) / 1000.0f;
        const float currentTime = static_cast<float>(SDL_GetTicks()) / 1000.0f;
        mAccumulator += currentTime - lastTime;
        lastTime = currentTime;

        for(; mAccumulator > mTimeStep; mAccumulator -= mTimeStep)
            for(Updateable *updateable : mUpdateableList)
                if(updateable->mActive)
                    updateable->Update(mTimeStep);
    }

    void EventHandler::AddUpdateable(Updateable *updateable)
    {
        mUpdateableList.push_back(updateable);
    }

    void EventHandler::RemoveUpdateable(const Updateable *updateable)
    {
        mUpdateableList.erase(find(mUpdateableList.begin(), mUpdateableList.end(), updateable));
    }

    void EventHandler::SetQuit()
    {
        mQuit = true;
    }

    bool EventHandler::IsCursorFree()
    {
        return mIsCursorFree;
    }

}
