
#include <Engine/Utils/UniformBuffers.h>
#include <GL/glew.h>

namespace engine
{
    ViewRayDataUB::ViewRayDataUB(void)
        : UniformBuffer(GL_DYNAMIC_DRAW, "ViewRayData")
    {
    }

    MatricesUB::MatricesUB(void)
        : UniformBuffer(GL_DYNAMIC_DRAW, "Matrices")
    {
    }
}

