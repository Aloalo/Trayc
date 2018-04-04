
#include <Engine/Engine/RayTraceRenderPass.h>
#include <Engine/Engine/AssetLoader.h>
#include <Engine/Core/Camera.h>
#include <Engine/Utils/Setting.h>
#include <Engine/Utils/UniformBuffers.h>
#include <easylogging++.h>

using namespace glm;
using namespace std;
using namespace stdext;

#define MAKE_CONSTANT(v) Shader::Constant((#v), (v))

namespace engine
{
    static const int MAX_SPHERES = 125;

    RayTraceRenderPass::RayTraceRenderPass(void)
        : RenderPass("rtPass", GL_COLOR_BUFFER_BIT)
    {
    }

    void RayTraceRenderPass::AddSphere(const RTSphere &sphere)
    {
        assert(mSpheres.size() < MAX_SPHERES);
        mSpheres.push_back(sphere);
    }

    void RayTraceRenderPass::CompileShaders()
    {
        const Shader::Defines defines = {};
        const Shader::Constants constants = { MAKE_CONSTANT(MAX_SPHERES) };
        mRayTraceCombiner.Init(AssetLoader::Get().ShaderPath("RayTrace").data(), defines, constants);
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
        mDstFB.AddAttachment(GL_RGBA16F, GL_RGBA, GL_FLOAT); //Lighting out / x
        mDstFB.Compile();

        // Bind own textures to appropriate slots
        mDstFB.GetAttachment(0).BindToSlot(TextureType::FINAL_SLOT);
    }

    void RayTraceRenderPass::Destroy()
    {
        mDstFB.Destroy();
        mRayTraceCombiner.Destroy();
    }

    void RayTraceRenderPass::UploadToGPU() const
    {
        UniformBuffers::Get().Primitives().spheres(mSpheres);
    }

    void RayTraceRenderPass::Render(const RenderingContext &rContext) const
    {
        UploadToGPU();

        const Camera &cam = *rContext.mCamera;
        const float halfTanFov = tanf(radians(cam.mFoV)) * 0.5f;

        const Program &p = mRayTraceCombiner.Prog();
        p.Use();
        p.SetUniform("cameraPos", cam.mPosition);
        p.SetUniform("U", cam.GetRight() * halfTanFov * cam.mAspectRatio);
        p.SetUniform("V", cam.GetUp() * halfTanFov);
        p.SetUniform("W", cam.GetDirection());
        p.SetUniform("ctSpheres", static_cast<int>(mSpheres.size()));

        mRayTraceCombiner.Draw();
    }
}
