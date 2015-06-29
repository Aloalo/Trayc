/*
* Copyright (c) 2014 Jure Ratkovic
*/
#include <Engine/Engine/Component.h>
#include <Engine/Engine/Entity.h>

namespace engine
{
    Component::Component(ComponentType type, Entity *entity)
        : mType(type), mEntity(entity)
    {
    }
}
