/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_SCENE_H
#define EN_SCENE_H

#include <vector>
#include <Engine/Engine/Entity.h>

namespace engine
{
    enum MessageType
    {
        MESSAGE_NONE = 0,
        MESSAGE_RENDER = 1 << 0,
        MESSAGE_UPDATE = 1 << 1,
    };

    class Scene
    {
    public:
        Scene();
    private:
        std::vector<Entity*> mEntities;
    };
}

#endif
