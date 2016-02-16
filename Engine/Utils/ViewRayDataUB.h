#ifndef EN_VIEW_RAY_DATA_UB_H
#define EN_VIEW_RAY_DATA_UB_H

#include <Engine/GL/UniformBuffer.h>
#include <glm/glm.hpp>

namespace engine
{
    class ViewRayDataUB : public UniformBuffer
    {
    public:
        ViewRayDataUB(void);

        UNIFORM_MACRO(float, tanHalfFovy, 0); // tan(fovy / 2)
        UNIFORM_MACRO(float, aspectTanHalfFovy, 4); // aspectRatio * tan(fovy / 2)
    };
}

#endif
