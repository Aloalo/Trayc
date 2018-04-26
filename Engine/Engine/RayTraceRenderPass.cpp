
#include <Engine/Engine/RayTraceRenderPass.h>
#include <Engine/Engine/AssetLoader.h>
#include <Engine/Core/Camera.h>
#include <Engine/Utils/Setting.h>
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
		const int ctRect = static_cast<int>(mRectangles.size());
		int ctReflectiveRect = 0;
		for (; ctReflectiveRect < ctRect; ++ctReflectiveRect) {
			if (mRectangles[ctReflectiveRect].materialData.y == 0.0f) {
				break;
			}
		}

        const Shader::Defines defines = {};
        const Shader::Constants constants = {
            MAKE_CONSTANT(CT_SPHERES, mSpheres.size()),
            MAKE_CONSTANT(CT_LIGHTS, mLights.size()),
            MAKE_CONSTANT(CT_RECTANGLES, mRectangles.size()),
            MAKE_CONSTANT(CT_BOXES, mBoxes.size()),
            MAKE_CONSTANT(CT_REFLECTIVE_RECT, ctReflectiveRect),
        };
        mRayTraceCombiner.Init(AssetLoader::Get().ShaderPath("RayTrace").data(), defines, constants);

        const Program &p = mRayTraceCombiner.Prog();
        p.Use();
		p.SetUniform("ambientColor", vec3(0.05f));
        p.SetUniform("lightFallofFactor", 5.0f);

        Program::Unbind();
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

        // Bind own textures to appropriate slots
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

        const float halfTanFov = tanf(radians(cam.mFoV)) * 0.5f;

        const Program &p = mRayTraceCombiner.Prog();
        p.Use();
        p.SetUniform("cameraPos", cam.mPosition);
        p.SetUniform("U", cam.GetRight() * halfTanFov * cam.mAspectRatio);
        p.SetUniform("V", cam.GetUp() * halfTanFov);
        p.SetUniform("W", cam.GetDirection());

        mRayTraceCombiner.Draw();
    }
}
