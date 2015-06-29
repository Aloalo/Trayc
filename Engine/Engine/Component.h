/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_COMPONENT_H
#define EN_COMPONENT_H

#include <glm/glm.hpp>

namespace engine
{
    class Entity;

    enum MessageType;

    enum ComponentType
    {
        COMPONENT_NONE = 0,
        COMPONENT_TRANSFORM = 1 << 0,
        COMPONENT_RENDERABLE = 1 << 1,
        COMPONENT_PHYSICS = 1 << 2,
        COMPONENT_INPUT = 1 << 3
    };

    class Component
    {
    public:
        Component(ComponentType type, Entity *entity);

        virtual void HandleMessage(MessageType message) = 0;

    private:
        ComponentType mType;
        Entity *mEntity;
    };
}

#endif
