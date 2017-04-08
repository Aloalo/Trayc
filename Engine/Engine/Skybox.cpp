
#include <Engine/Engine/Skybox.h>
#include <Engine/Geometry/GeometryMeshes.h>
#include <Engine/Engine/AssetLoader.h>
#include <Engine/Engine/Renderer.h>
#include <Engine/Core/Camera.h>
#include <Engine/Utils/TextureEffects.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

namespace engine
{
    Skybox::Skybox(void)
        : mSkyboxVA(GL_STATIC_DRAW), mFarPlaneMod(3.0f)
    {
    }

    void Skybox::Init(const Renderer *renderer)
    {
        Destroy();

        mSkyboxProg.Init(AssetLoader::Get().ShaderPath("F_Skybox").data());
        mSkyboxProg.Use();
        mSkyboxProg.SetUniform("skybox", TextureType::SKYBOX_SLOT);
        Program::Unbind();

        // Load skyboxes
        const int ctSkyboxes = 4;

        for(int i = 0; i < ctSkyboxes; ++i) {
            mSkyboxCubemaps.push_back(CubemapTexture());

            const string skyboxFolder = "skybox" + to_string(i) + "/";
            const string sideNames[6] = {
                AssetLoader::Get().TexturePath(skyboxFolder + "back.jpg"),
                AssetLoader::Get().TexturePath(skyboxFolder + "front.jpg"),
                AssetLoader::Get().TexturePath(skyboxFolder + "bottom.jpg"),
                AssetLoader::Get().TexturePath(skyboxFolder + "top.jpg"),
                AssetLoader::Get().TexturePath(skyboxFolder + "left.jpg"),
                AssetLoader::Get().TexturePath(skyboxFolder + "right.jpg"),
            };
            mSkyboxCubemaps[i].Init(sideNames, TextureType::DIFFUSE_MAP);
            // Create the irradiance map
            mIrradianceMaps.push_back(TextureEffects::Get().GenerateIrradianceMap(mSkyboxCubemaps[i]));
        }

        mSkyboxCubemaps[0].BindToSlot(TextureType::SKYBOX_SLOT);
        mIrradianceMaps[0].BindToSlot(TextureType::IRRADIANCE_SLOT);

        const float sideHalfSize = renderer->GetCamera()->mFarDistance * mFarPlaneMod * 0.5f;
        mSkyboxScale =  scale(mat4(1.0f), vec3(sideHalfSize));
        mSkyboxTransform = mat4(1.0f);
        TriangleMesh cube = GetCubeMeshSolid(true, false);
        cube.FlipWinding();
        mSkyboxVA.Init(&cube);
    }

    void Skybox::LoadSkybox(int skyboxIdx)
    {
        mSkyboxCubemaps[skyboxIdx].BindToSlot(TextureType::SKYBOX_SLOT);
        mIrradianceMaps[skyboxIdx].BindToSlot(TextureType::IRRADIANCE_SLOT);
    }

    void Skybox::Destroy()
    {
        mSkyboxProg.Destroy();
        for(auto &t : mSkyboxCubemaps) {
            t.Destroy();
        }
        for(auto &t : mIrradianceMaps) {
            t.Destroy();
        }
        mSkyboxVA.Destroy();
    }

    void Skybox::Draw(const Camera *camera) const
    {
        Camera skyBoxCam(*camera);
        skyBoxCam.mFarDistance *= mFarPlaneMod;
        mat4 V = skyBoxCam.GetViewMatrix();
        V[3][0] = V[3][1] = V[3][2] = 0.0f;
        const mat4 VP = skyBoxCam.GetProjectionMatrix() * V;

        glDepthRange(1.0, 1.0);
        mSkyboxProg.Use();
        mSkyboxProg.SetUniform("MVP", VP * mSkyboxTransform * mSkyboxScale);
        mSkyboxVA.Render(GL_TRIANGLES);
        glDepthRange(0.0, 1.0);
    }

    const mat4& Skybox::GetTransform() const
    {
        return mSkyboxTransform;
    }
}
