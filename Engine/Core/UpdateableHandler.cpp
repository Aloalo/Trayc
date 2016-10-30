#include <Engine/Core/UpdateableHandler.h>
#include <Engine/Utils/StlExtensions.hpp>
#include <ctime>

using namespace stdext;

namespace engine
{
    UpdateableHandler::UpdateableHandler(float timeStep)
        : mTimeStep(timeStep), mAccumulator(0.0f)
    {
    }

    void UpdateableHandler::Update()
    {
        static float lastTime = static_cast<float>(clock()) / CLOCKS_PER_SEC;
        const float currentTime = static_cast<float>(clock()) / CLOCKS_PER_SEC;
        mAccumulator += currentTime - lastTime;
        lastTime = currentTime;

        for(; mAccumulator >= mTimeStep; mAccumulator -= mTimeStep)
            for(Updateable *updateable : mUpdateableList)
                if(updateable->mActive)
                    updateable->Update(mTimeStep);
    }

    void UpdateableHandler::AddUpdateable(Updateable *updateable)
    {
        mUpdateableList.push_back(updateable);
    }

    void UpdateableHandler::RemoveUpdateable(const Updateable *updateable)
    {
        erase(mUpdateableList, updateable);
    }
}
