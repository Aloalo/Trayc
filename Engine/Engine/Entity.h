/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_ENTITY_H
#define EN_ENTITY_H

#include <Engine/Engine/Component.h>
#include <Engine/Engine/Scene.h>

namespace engine
{
    class Entity
    {
        Entity(Scene *scene);

        void HandleMessage(MessageType message);
        void AddComponent(Component *componenet);
        void RemoveComponent(Component *componenet);

    private:
        Scene *mScene;
        std::vector<Component*> mComponents;
    };
}

#endif
