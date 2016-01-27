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

    vec3 AABB::Size() const
    {
        return mMaxv - mMinv;
    }

    std::array<glm::vec3, 6> AABB::Vertices() const
    {
        const vec3 sz = Size();
        array<vec3, 6> vertices;

        vertices[0] = mMinv + vec3(0.0f, 0.0f, 0.0f);
        vertices[1] = mMinv + vec3(sz.x, 0.0f, 0.0f);
        vertices[2] = mMinv + vec3(0.0f, sz.y, 0.0f);
        vertices[3] = mMinv + vec3(0.0f, 0.0f, sz.z);
        vertices[4] = mMaxv - vec3(0.0f, 0.0f, 0.0f);
        vertices[5] = mMaxv - vec3(sz.x, 0.0f, 0.0f);
        vertices[6] = mMaxv - vec3(0.0f, sz.y, 0.0f);
        vertices[7] = mMaxv - vec3(0.0f, 0.0f, sz.z);

        return vertices;
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

