
#include <Engine/Utils/UniformBuffers.h>
#include <GL/glew.h>
#include <easylogging++.h>

using namespace glm;

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

    PrimitivesUB::PrimitivesUB(void)
        : UniformBuffer(GL_STREAM_DRAW, "Primitives")
    {
    }


    UniformBuffers::UniformBuffers(void)
    {
        mViewRayData.Init(2 * sizeof(vec4));
        mMatrices.Init(6 * sizeof(mat4));

        const auto primitivesSize = PrimitivesUB::MAX_SPHERES * sizeof(RTSphere) +
            PrimitivesUB::MAX_LIGHTS * sizeof(RTLight) +
            PrimitivesUB::MAX_RECTANGLES * sizeof(RTRectangle) +
            PrimitivesUB::MAX_BOXES * sizeof(RTBox);

        LOG(INFO) << "PrimitivesUB size: " << primitivesSize;

        mPrimitives.Init(primitivesSize);
    }

    void UniformBuffers::Destroy()
    {
        mViewRayData.Destroy();
        mMatrices.Destroy();
        mPrimitives.Destroy();
    }

    const ViewRayDataUB& UniformBuffers::ViewRayData() const
    {
        return mViewRayData;
    }

    const MatricesUB& UniformBuffers::Matrices() const
    {
        return mMatrices;
    }

    const PrimitivesUB& UniformBuffers::Primitives() const
    {
        return mPrimitives;
    }

    const UniformBuffer* UniformBuffers::GetUniformBuffer(const std::string &name) const
    {
        if(name == mViewRayData.GetName()) {
            return &mViewRayData;
        }
        if(name == mMatrices.GetName()) {
            return &mMatrices;
        }
        if (name == mPrimitives.GetName()) {
            return &mPrimitives;
        }
        return nullptr;
    }
}

