/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_EVENTHANDLER_H
#define EN_EVENTHANDLER_H

#include <Engine/Core/EventListener.h>
#include <Engine/Core/Updateable.h>
#include <vector>

namespace engine
{
    //Handles SDL event and dispatches them to EventListeners
    //Also handles per frame time updates
    class EventHandler
    {
    public:
        //event handling
        static void ProcessPolledEvents();
        static void AddEventListener(EventListener *listener);
        static void RemoveEventListener(const EventListener *listener);

        //Has quit event occured?
        static bool Quit();

        //update handling
        static void Update();
        static void AddUpdateable(Updateable *updateable);
        static void RemoveUpdateable(const Updateable *updateable);

    private:
        static bool quit;
        static bool isCursorFree;
        static float timeStep;
        static float accumulator;
        static std::vector<EventListener*> listenerList;

        static std::vector<Updateable*> updateableList;
    };
}

#endif
