/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Core/InputHandler.h>
#include <Engine/Core/SDLHandler.h>
#include <Engine/Utils/StlExtensions.hpp>

using namespace std;
using namespace stdext;

namespace engine
{
    InputHandler::InputHandler(void)
        : mIsCursorFree(true), mQuit(false)
    {
    }


    void InputHandler::ProcessPolledEvents()
    {
        SDL_Event testEvent;
        while(SDL_PollEvent(&testEvent))
        {
            if(testEvent.type == SDL_QUIT)
                mQuit = true;

            else if(testEvent.type == SDL_KEYDOWN && testEvent.key.keysym.sym == SDLK_LSHIFT)
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

            for(InputObserver *listener : mListenerList)
            {
                if(listener->mActive)
                {
                    listener->HandleEvent(testEvent);
                    switch(testEvent.type)
                    {
                    case SDL_KEYUP:
                    case SDL_KEYDOWN:
                        listener->KeyPress(testEvent.key);
                        break;
                    case SDL_MOUSEMOTION:
                        listener->MouseMotion(testEvent.motion);
                        break;
                    case SDL_WINDOWEVENT:
                        listener->WindowEvent(testEvent.window);
                        break;
                    case SDL_MOUSEBUTTONUP:
                    case SDL_MOUSEBUTTONDOWN:
                        listener->MouseButton(testEvent.button);
                        break;
                    case SDL_MOUSEWHEEL:
                        listener->MouseWheel(testEvent.wheel);
                        break;
                    case SDL_QUIT:
                        listener->QuitEvent(testEvent.quit);
                        break;
                    default:
                        break;
                    }
                }
            }

            if(mQuit == true)
                break;
        }
    }

    void InputHandler::AddEventListener(InputObserver *listener)
    {
        mListenerList.push_back(listener);
    }

    void InputHandler::RemoveEventListener(const InputObserver *listener)
    {
        erase(mListenerList, listener);
    }

    bool InputHandler::Quit() const
    {
        return mQuit;
    }

    void InputHandler::SetQuit()
    {
        mQuit = true;
    }

    bool InputHandler::IsCursorFree() const
    {
        return mIsCursorFree;
    }
}
