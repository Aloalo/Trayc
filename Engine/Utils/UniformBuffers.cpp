
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

    UniformBuffers::UniformBuffers(void)
    {
        mViewRayData.Init(2 * sizeof(float));
        mMatrices.Init(6 * sizeof(glm::mat4));
    }

    void UniformBuffers::Destroy()
    {
        mViewRayData.Destroy();
        mMatrices.Destroy();
    }

    const ViewRayDataUB& UniformBuffers::ViewRayData() const
    {
        return mViewRayData;
    }

    const MatricesUB& UniformBuffers::Matrices() const
    {
        return mMatrices;
    }

    const UniformBuffer* UniformBuffers::GetUniformBuffer(const std::string &name) const
    {
        if(name == mViewRayData.GetName()) {
            return &mViewRayData;
        }
        if(name == mMatrices.GetName()) {
            return &mMatrices;
        }
    }
}

