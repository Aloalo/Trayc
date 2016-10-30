
#ifndef EN_AABB_H
#define EN_AABB_H

#include <glm/glm.hpp>
#include <array>

namespace engine
{
    struct AABB
    {
        AABB(void);
        AABB(const glm::vec3 &minv, const glm::vec3 &maxv);

        AABB& operator|=(const glm::vec3 &p);
        AABB& operator|=(const AABB &box);

        template<int N>
        AABB& operator|=(const std::array<glm::vec3, N> points)
        {
            for(const glm::vec3 &p : points)
            {
                mMinv = glm::min(mMinv, p);
                mMaxv = glm::max(mMaxv, p);
            }
            return *this;
        }

        glm::vec3 Center() const;
        glm::vec3 Size() const;
        std::array<glm::vec3, 8> Vertices() const;
        glm::vec3 GetPositiveVertex(const glm::vec3 &n) const;

        glm::vec3 mMinv;
        glm::vec3 mMaxv;
    };

    AABB operator|(const AABB &box, const glm::vec3 &p);
    AABB operator|(const AABB &box1, const AABB &box2);
}

#endif
