/*
* Copyright (c) 2014 Jure Ratkovic
*/
#include <Engine/Engine/Entity.h>
#include <Engine/Utils/StlExtensions.hpp>

using namespace stdext;

namespace engine
{
    Entity::Entity(Scene *scene)
        : mScene(scene)
    {
    }

    void Entity::AddComponent(Component *componenet)
    {
        mComponents.push_back(componenet);
    }

    void Entity::RemoveComponent(Component *componenet)
    {
        erase(mComponents, componenet);
    }

    void Entity::HandleMessage(MessageType message)
    {
        for(Component *component : mComponents)
            component->HandleMessage(message);
    }

}
