
#include <Engine/Engine/RayTraceRenderPass.h>
#include <Engine/Engine/AssetLoader.h>
#include <Engine/Utils/StlExtensions.hpp>
#include <Engine/Engine/ShadowRenderPass.h>
#include <Engine/Engine/GeometryRenderPass.h>
#include <Engine/Engine/ForwardRenderPass.h>
#include <Engine/Engine/Renderer.h>
#include <Engine/Utils/Setting.h>
#include <Engine/Utils/Utilities.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;
using namespace stdext;

namespace engine
{
    RayTraceRenderPass::RayTraceRenderPass(void)
        : RenderPass("rtPass", GL_COLOR_BUFFER_BIT)
    {
    }

    void RayTraceRenderPass::CompileShaders()
    {
        mRayTraceCombiner.Init(AssetLoader::Get().ShaderPath("RayTrace").data());
        Program::Unbind();
    }

    void RayTraceRenderPass::Init()
    {
        const int width = Setting<int>("screenWidth");
        const int height = Setting<int>("screenHeight");

        // Init shader
        CompileShaders();

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

    void RayTraceRenderPass::Render(const RenderingContext &rContext) const
    {
        const Camera &cam = *rContext.mCamera;
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
