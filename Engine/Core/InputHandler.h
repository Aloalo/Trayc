#ifndef EN_INPUTHANDLER_H
#define EN_INPUTHANDLER_H

#include <Engine/Core/InputObserver.h>
#include <Engine/Core/Updateable.h>
#include <vector>

namespace engine
{
    //Handles SDL events and dispatches them to InputObservers
    class InputHandler
    {
    public:
        InputHandler(void);

        //Input event handling
        void AddEventListener(InputObserver *listener);
        void RemoveEventListener(const InputObserver *listener);

        //Has quit event occured?
        bool Quit() const;
        //Set quit event
        void SetQuit();
        //Is cursor free?
        bool IsCursorFree() const;
        void SetCursorFree(bool cursorFree);

    private:
        friend class Game;

        void ProcessPolledEvents();

        bool mQuit;
        bool mIsCursorFree;

        std::vector<InputObserver*> mListenerList;
    };
}

#endif
