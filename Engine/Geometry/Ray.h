#ifndef EN_RAY_H
#define EN_RAY_H

#include <glm/glm.hpp>

namespace engine
{
    struct Ray
    {
        glm::vec3 mOrigin;
        glm::vec3 mDirection;
    };
}


#endif
