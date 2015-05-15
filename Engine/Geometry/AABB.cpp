/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Geometry/AABB.h>
#include <limits>

using namespace std;
using namespace glm;

namespace engine
{
    AABB::AABB(void)
        : minv(numeric_limits<float>::max()), maxv(numeric_limits<float>::lowest())
    {
    }

    AABB& AABB::operator|=(const glm::vec3 &p)
    {
        minv = glm::min(minv, p);
        maxv = glm::max(maxv, p);
        return *this;
    }

    AABB& AABB::operator|=(const AABB &box)
    {
        minv = glm::min(minv, box.minv);
        maxv = glm::max(maxv, box.maxv);
        return *this;
    }

    engine::AABB operator|(const AABB &box, const glm::vec3 &p)
    {
        AABB ret(box);
        ret |= p;
        return ret;
    }

    engine::AABB operator|(const AABB &box1, const AABB &box2)
    {
        AABB ret(box1);
        ret |= box2;
        return ret;
    }
}

