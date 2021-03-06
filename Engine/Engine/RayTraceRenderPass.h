
#ifndef EN_RAY_TRACE_RENDER_PASS_H
#define EN_RAY_TRACE_RENDER_PASS_H

#include <Engine/Engine/RenderPass.h>
#include <Engine/Engine/TextureCombiner.h>
#include <Engine/Geometry/RayTracePrimitives.h>

namespace engine
{
    class RayTraceRenderPass : public RenderPass
    {
    public:
        RayTraceRenderPass(void);

        virtual void Init() override;
        virtual void Destroy() override;

        virtual void Render(const RenderingContext &rContext) const override;
        virtual void ResizeDstBuffer(int width, int height) override;

        void CompileShaders();
		void SetRayTraceQuality(int quality);

        void Clear();

        std::vector<RTSphere> mSpheres;
        std::vector<RTLight> mLights;
        std::vector<RTRectangle> mRectangles;
        std::vector<RTBox> mBoxes;

    private:
        void UploadToGPU(const Camera &cam) const;

        TextureCombiner mRayTraceCombiner;
		int mRayTraceQuality; // 1 to 10
    };
}

#endif
