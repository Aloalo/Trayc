/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_UPDATEABLEHANDLER_H
#define EN_UPDATEABLEHANDLER_H

#include <Engine/Core/Updateable.h>
#include <vector>

namespace engine
{
    //Handles time updateable objects
    class UpdateableHandler
    {
    public:
        UpdateableHandler(float timeStep);

        //Update handling
        void Update();
        void AddUpdateable(Updateable *updateable);
        void RemoveUpdateable(const Updateable *updateable);

    private:
        float mTimeStep;
        float mAccumulator;

        std::vector<Updateable*> mUpdateableList;
    };
}

#endif
