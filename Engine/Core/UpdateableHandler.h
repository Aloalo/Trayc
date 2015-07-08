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

        void AddUpdateable(Updateable *updateable);
        void RemoveUpdateable(const Updateable *updateable);

    private:
        friend class Scene;
        //Update handling
        void Update();

        float mTimeStep;
        float mAccumulator;

        std::vector<Updateable*> mUpdateableList;
    };
}

#endif
