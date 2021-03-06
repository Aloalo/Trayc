#ifndef EN_VIEW_RAY_DATA_UB_H
#define EN_VIEW_RAY_DATA_UB_H

#include <Engine/GL/UniformBuffer.h>
#include <Engine/Utils/Singleton.h>
#include <Engine/Geometry/RayTracePrimitives.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace engine
{
    class ViewRayDataUB : public UniformBuffer
    {
    public:
        ViewRayDataUB(void);

        UNIFORM_MACRO(glm::vec4, fovData, 0); // tan(fovy / 2) :: aspectRatio * tan(fovy / 2) :: - :: -
        UNIFORM_MACRO(glm::vec4, cameraDist, sizeof(glm::vec4)); // near :: far :: far-near :: 1/(far-near)
    };

    class MatricesUB : public UniformBuffer
    {
    public:
        MatricesUB(void);

        UNIFORM_MACRO(glm::mat4, V, 0 * sizeof(glm::mat4));
        UNIFORM_MACRO(glm::mat4, P, 1 * sizeof(glm::mat4));
        UNIFORM_MACRO(glm::mat4, VP, 2 * sizeof(glm::mat4));
        UNIFORM_MACRO(glm::mat4, invV, 3 * sizeof(glm::mat4));
        UNIFORM_MACRO(glm::mat4, invP, 4 * sizeof(glm::mat4));
        UNIFORM_MACRO(glm::mat4, invVP, 5 * sizeof(glm::mat4));
    };

    class PrimitivesUB : public UniformBuffer
    {
    public:
        PrimitivesUB(void);

        static const int MAX_SIZE = 20 * 1024;

        UNIFORM_ARRAY_MACRO(std::vector<RTRectangle>, rectangles);
        UNIFORM_ARRAY_MACRO(std::vector<RTBox>, boxes);
        UNIFORM_ARRAY_MACRO(std::vector<RTSphere>, spheres);
        UNIFORM_ARRAY_MACRO(std::vector<RTLight>, lights);
    };

    class UniformBuffers : public Singleton<UniformBuffers>
    {
    public:
        void Destroy();
        const ViewRayDataUB& ViewRayData() const;
        const MatricesUB& Matrices() const;
        const PrimitivesUB& Primitives() const;
        const UniformBuffer* GetUniformBuffer(const std::string &name) const;

    private:
        friend struct Singleton<UniformBuffers>;
        UniformBuffers(void);

        ViewRayDataUB mViewRayData;
        MatricesUB mMatrices;
        PrimitivesUB mPrimitives;
    };
}

#endif
