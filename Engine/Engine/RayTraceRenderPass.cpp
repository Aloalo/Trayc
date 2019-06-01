
#include <Engine/Engine/RayTraceRenderPass.h>
#include <Engine/Engine/AssetLoader.h>
#include <Engine/Engine/Renderer.h>
#include <Engine/Core/Camera.h>
#include <Engine/Utils/Setting.h>
#include <Engine/Utils/TimerQuery.h>
#include <Engine/Utils/UniformBuffers.h>
#include <easylogging++.h>

using namespace glm;
using namespace std;
using namespace stdext;

#define MAKE_CONSTANT(n, v) Shader::Constant((#n), (v))

namespace engine
{
    RayTraceRenderPass::RayTraceRenderPass(void)
        : RenderPass("rtPass", GL_COLOR_BUFFER_BIT), mRayTraceQuality(Setting<int>("rayTraceQuality"))
    {
    }

    void RayTraceRenderPass::CompileShaders()
    {
		const int ctRectangles = static_cast<int>(mRectangles.size());

		for (const RTRectangle &r : mRectangles) {
			if (r.p2.w <= 0.0f) {
                throw exception("Non reflective rectangle found");
			}
		}

        const Shader::Constants constants = {
            MAKE_CONSTANT(CT_SPHERES, mSpheres.size()),
            MAKE_CONSTANT(CT_LIGHTS, mLights.size()),
            MAKE_CONSTANT(CT_RECTANGLES, ctRectangles),
            MAKE_CONSTANT(CT_BOXES, mBoxes.size()),
        };
        mRayTraceCombiner.Init(AssetLoader::Get().ShaderPath("RayTrace").data(), Shader::Defines{}, constants);
        //mRayTraceCombiner.Init(AssetLoader::Get().ShaderPath("RayTrace").data(),AssetLoader::Get().ShaderPath("RayTrace_unoptimized").data(), Shader::Defines{}, constants);

        const Program &p = mRayTraceCombiner.Prog();
		p.SetUniform("ambientColor", vec3(0.05f));
        p.SetUniform("lightFallofFactor", 5.0f);
    }

    void RayTraceRenderPass::ResizeDstBuffer(int width, int height)
    {
		mDstFB.Resize((width * mRayTraceQuality) / 10, (height * mRayTraceQuality) / 10);
    }

	void RayTraceRenderPass::SetRayTraceQuality(int quality)
	{
		assert(quality > 0 && quality <= 10);
		mRayTraceQuality = quality;
		ResizeDstBuffer(mRenderer->ScreenSize().x, mRenderer->ScreenSize().y);
	}

    void RayTraceRenderPass::Clear()
    {
        mSpheres.clear();
        mLights.clear();
        mRectangles.clear();
        mBoxes.clear();
    }

    void RayTraceRenderPass::Init()
    {
        GLint value = -1;
        glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &value);
        LOG(INFO) << "[RayTraceRenderPass::Init] GL_MAX_FRAGMENT_UNIFORM_COMPONENTS: " << value;

        glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &value);
        LOG(INFO) << "[RayTraceRenderPass::Init] GL_MAX_UNIFORM_BLOCK_SIZE: " << value;

        // Init buffer
		const int w = mRenderer->ScreenSize().x;
		const int h = mRenderer->ScreenSize().y;

		mDstFB.Init((w * mRayTraceQuality) / 10, (h * mRayTraceQuality) / 10);
        mDstFB.AddAttachment(GL_RGB16F, GL_RGB, GL_FLOAT); //Lighting out
        mDstFB.Compile();
		mDstFB.GetAttachment(0).BindToSlot(TextureType::FINAL_SLOT);
    }

    void RayTraceRenderPass::Destroy()
    {
        mDstFB.Destroy();
        mRayTraceCombiner.Destroy();
    }

    void RayTraceRenderPass::UploadToGPU(const Camera &cam) const
    {
        const auto primitivesUB = UniformBuffers::Get().Primitives();
		size_t offset = 0;
        primitivesUB.rectangles(mRectangles, offset);
		offset += mRectangles.size() * sizeof(RTRectangle);
        primitivesUB.boxes(mBoxes, offset);
		offset += mBoxes.size() * sizeof(RTBox);
		primitivesUB.spheres(mSpheres, offset);
		offset += mSpheres.size() * sizeof(RTSphere);
		primitivesUB.lights(mLights, offset);

		offset += mLights.size() * sizeof(RTLight);

		if (offset > PrimitivesUB::MAX_SIZE) {
			throw exception("PrimitivesUB size larger then max: " + offset);
		}
    }

    void RayTraceRenderPass::Render(const RenderingContext &rContext) const
    {
        const Camera &cam = *rContext.mCamera;
        UploadToGPU(cam);

        const float halfTanFov = tanf(radians(cam.mFoV * 0.5f));

        const Program &p = mRayTraceCombiner.Prog();
        p.SetUniform("cameraPos", cam.mPosition);
        p.SetUniform("U", cam.GetRight() * halfTanFov * cam.mAspectRatio);
        p.SetUniform("V", cam.GetUp() * halfTanFov);
        p.SetUniform("W", cam.GetDirection());

		{
			TimerQuery t("mRayTraceCombiner");
			mRayTraceCombiner.Draw();
		}
    }
}
