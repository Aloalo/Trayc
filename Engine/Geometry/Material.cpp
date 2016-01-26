/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Geometry/Material.h>

namespace engine
{
    Material::Material(void)
        : mID(-1), mKd(1.0f), mKs(1.0f), mGloss(1.0f)
    {
    }
}
