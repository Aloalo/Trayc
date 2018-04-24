
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

    void RayTraceRenderPass::AddSphere(const RTSphere &sphere)
    {
        if (mSpheres.size() > PrimitivesUB::MAX_SPHERES) {
            throw exception("Exceeded MAX_SPHERES");
        }
        mSpheres.push_back(sphere);
    }

    void RayTraceRenderPass::AddLight(const RTLight &light)
    {
        if (mLights.size() > PrimitivesUB::MAX_LIGHTS) {
            throw exception("Exceeded MAX_LIGHTS");
        }
        mLights.push_back(light);
    }

    void RayTraceRenderPass::AddRectangle(const RTRectangle &rect)
    {
        if (mRectangles.size() > PrimitivesUB::MAX_RECTANGLES) {
            throw exception("Exceeded MAX_RECTANGLES");
        }
        mRectangles.push_back(rect);
    }

    void RayTraceRenderPass::AddBox(const RTBox &box)
    {
        if (mBoxes.size() > PrimitivesUB::MAX_BOXES) {
            throw exception("Exceeded MAX_BOXES");
        }
        mBoxes.push_back(box);
    }

    void RayTraceRenderPass::CompileShaders()
    {
        const Shader::Defines defines = {};
        const Shader::Constants constants = {
            MAKE_CONSTANT(MAX_SPHERES, PrimitivesUB::MAX_SPHERES),
            MAKE_CONSTANT(MAX_LIGHTS, PrimitivesUB::MAX_LIGHTS),
            MAKE_CONSTANT(MAX_RECTANGLES, PrimitivesUB::MAX_RECTANGLES),
            MAKE_CONSTANT(MAX_BOXES, PrimitivesUB::MAX_BOXES)
        };
        mRayTraceCombiner.Init(AssetLoader::Get().ShaderPath("RayTrace").data(), defines, constants);

        const Program &p = mRayTraceCombiner.Prog();
        p.Use();
        p.SetUniform("ambientColor", vec3(0.05f));
        p.SetUniform("lightFallofFactor", 50.0f);

        Program::Unbind();
    }

    void RayTraceRenderPass::Init()
    {
        const int width = Setting<int>("screenWidth");
        const int height = Setting<int>("screenHeight");

        // Init shader
        CompileShaders();

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
        primitivesUB.rectangles(mRectangles);
        primitivesUB.boxes(mBoxes);
        primitivesUB.spheres(mSpheres);
        primitivesUB.lights(mLights);
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
        p.SetUniform("ctSpheres", static_cast<int>(mSpheres.size()));
        p.SetUniform("ctLights", static_cast<int>(mLights.size()));
        p.SetUniform("ctRectangles", static_cast<int>(mRectangles.size()));
        p.SetUniform("ctBoxes", static_cast<int>(mBoxes.size()));

        mRayTraceCombiner.Draw();
    }
}
