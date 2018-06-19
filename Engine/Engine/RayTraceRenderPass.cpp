
#include <Engine/Engine/RayTraceRenderPass.h>
#include <Engine/Engine/AssetLoader.h>
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
        : RenderPass("rtPass", GL_COLOR_BUFFER_BIT)
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

        const Shader::Defines defines = (mCheckerboarding ? Shader::Defines{"CHECKERBOARDING"} : Shader::Defines{});
        const Shader::Constants constants = {
            MAKE_CONSTANT(CT_SPHERES, mSpheres.size()),
            MAKE_CONSTANT(CT_LIGHTS, mLights.size()),
            MAKE_CONSTANT(CT_RECTANGLES, ctRectangles),
            MAKE_CONSTANT(CT_BOXES, mBoxes.size()),
        };
        mRayTraceCombiner.Init(AssetLoader::Get().ShaderPath("RayTrace").data(), defines, constants);
        //mRayTraceCombiner.Init(AssetLoader::Get().ShaderPath("RayTrace").data(), AssetLoader::Get().ShaderPath("RayTrace_unoptimized").data(), defines, constants);

        const Program &p = mRayTraceCombiner.Prog();
		p.SetUniform("ambientColor", vec3(0.05f));
        p.SetUniform("lightFallofFactor", 5.0f);
    }

    void RayTraceRenderPass::ResizeDstBuffer(int width, int height)
    {
        if (mCheckerboarding) {
            mReconstructionFB.Resize(width, height);
            width /= 2;
        }
        mDstFB.Resize(width, height);
    }

    void RayTraceRenderPass::SetCheckerboarding(bool flag)
    {
        assert(mCheckerboarding != flag);

        mCheckerboarding = flag;
        if (flag) {
            const int width = mDstFB.Width();
            const int height = mDstFB.Height();

            mDstFB.Resize(width / 2, height);
            mDstFB.GetAttachment(0).BindToSlot(TextureType::CHECKERED);
            
            mReconstructionFB.Init(width, height);
            mReconstructionFB.AddAttachment(GL_RGB16F, GL_RGB, GL_FLOAT); //Lighting out
            mReconstructionFB.Compile();

            mReconstructionFB.GetAttachment(0).BindToSlot(TextureType::FINAL_SLOT);

            mReconstructionCombiner.Init(AssetLoader::Get().ShaderPath("C_TexToScreen").data(), AssetLoader::Get().ShaderPath("C_CheckerCombiner").data(), Shader::Defines());
            mReconstructionCombiner.Prog().SetUniform("tex", TextureType::CHECKERED);
        }
        else {
            mReconstructionFB.Destroy();
            mReconstructionCombiner.Destroy();

            mDstFB.Resize(mDstFB.Width() * 2, mDstFB.Height());
            mDstFB.GetAttachment(0).BindToSlot(TextureType::FINAL_SLOT);
        }
    }

    void RayTraceRenderPass::ToggleCheckerboarding()
    {
        SetCheckerboarding(!mCheckerboarding);
        CompileShaders();
    }

    void RayTraceRenderPass::Init()
    {
        const int width = Setting<int>("screenWidth");
        const int height = Setting<int>("screenHeight");

        GLint value = -1;
        glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &value);
        LOG(INFO) << "[RayTraceRenderPass::Init] GL_MAX_FRAGMENT_UNIFORM_COMPONENTS: " << value;

        glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &value);
        LOG(INFO) << "[RayTraceRenderPass::Init] GL_MAX_UNIFORM_BLOCK_SIZE: " << value;

        // Init buffer
        mDstFB.Init(width, height);
        mDstFB.AddAttachment(GL_RGB16F, GL_RGB, GL_FLOAT); //Lighting out
        mDstFB.Compile();

        SetCheckerboarding(Setting<bool>("checkerboarding"));
    }

    void RayTraceRenderPass::Destroy()
    {
        mDstFB.Destroy();
        mReconstructionFB.Destroy();

        mRayTraceCombiner.Destroy();
        mReconstructionCombiner.Destroy();
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

        const float halfTanFov = tanf(radians(cam.mFoV)) * 0.5f;

        const Program &p = mRayTraceCombiner.Prog();
        p.SetUniform("cameraPos", cam.mPosition);
        p.SetUniform("U", cam.GetRight() * halfTanFov * cam.mAspectRatio);
        p.SetUniform("V", cam.GetUp() * halfTanFov);
        p.SetUniform("W", cam.GetDirection());

        if (mCheckerboarding) {
            p.SetUniform("invTexWidth", 0.5f / static_cast<float>(mDstFB.Width()));
        }

        {
            TimerQuery t("mRayTraceCombiner");
            mRayTraceCombiner.Draw();
        }

        if (mCheckerboarding) {
            TimerQuery t("mReconstructionCombiner");
            mReconstructionFB.Bind();
            glViewport(0, 0, mReconstructionFB.Width(), mReconstructionFB.Height());
            mReconstructionCombiner.Draw();
        }
    }
}
