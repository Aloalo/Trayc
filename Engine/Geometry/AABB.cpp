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
        : mMinv(numeric_limits<float>::max()), mMaxv(numeric_limits<float>::lowest())
    {
    }

    AABB::AABB(const glm::vec3 &minv, const glm::vec3 &maxv)
        : mMinv(minv), mMaxv(maxv)
    {
    }

    AABB& AABB::operator|=(const glm::vec3 &p)
    {
        mMinv = glm::min(mMinv, p);
        mMaxv = glm::max(mMaxv, p);
        return *this;
    }

    AABB& AABB::operator|=(const AABB &box)
    {
        mMinv = glm::min(mMinv, box.mMinv);
        mMaxv = glm::max(mMaxv, box.mMaxv);
        return *this;
    }

    AABB operator|(const AABB &box, const glm::vec3 &p)
    {
        return AABB(box) |= p;
    }

    AABB operator|(const AABB &box1, const AABB &box2)
    {
        return AABB(box1) |= box2;
    }
}

