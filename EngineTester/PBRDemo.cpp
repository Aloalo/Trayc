#include "PBRDemo.h"
#include <Engine/Geometry/GeometryMeshes.h>
#include <Engine/Engine/AssetLoader.h>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include <iostream>


using namespace engine;
using namespace glm;
using namespace std;

PBRDemo::PBRDemo(void)
    : mLight(vec3(0.0f), vec3(1.0f), true, vec3(0.0f, 1.0f, 1.0f))
{
}

void PBRDemo::Init(RasterizedGame &game, Scene &scene)
{
#ifdef _DEBUG
    const int sphereQuality = 3;
#else
    const int sphereQuality = 4;
#endif

    const float radius = 10.0f;
    TriangleMesh sphere = GetSphereMeshSolid(false, sphereQuality, radius);
    //TriangleMesh sphere = GetCubeMeshSolid(false, true, scale(mat4(1.0f), vec3(radius)));
    scene.mTriMeshes.push_back(sphere);
    const int sphereMeshIdx = static_cast<int>(scene.mTriMeshes.size()) - 1;

    const float spacing = 2.5f * radius;
    const int ctBalls = 10;
    const float step = 1.0f / float(ctBalls);
    int i = 0;
    for(float rough = 0.001f; i <= ctBalls; rough += step, ++i) {
        int j = 0;
        for(float metal = 0.001f; j <= ctBalls; metal += step, ++j) {
            Material mat;
            mat.mGloss = rough;
            mat.mKs = vec3(metal);
            mat.mKd = vec3(1.0f, 0.0f, 0.0f);
            mat.mNeedsForwardRender = false;
            scene.mMaterials.push_back(mat);
            const int matIdx = static_cast<int>(scene.mMaterials.size()) - 1;
            scene.AddObject(AssetLoader::Get().CreateObject(&scene, sphereMeshIdx, matIdx));
            auto object = &scene.mObjects3D.back();
            object->mShadowCaster = false;

            // Set transform
            const float offsetI = static_cast<float>(i - ctBalls / 2);
            const float offsetJ = static_cast<float>(j - ctBalls / 2);

            const mat4 T = translate(mat4(1.0f), vec3(offsetI * spacing, 0.0f, offsetJ * spacing));
            object->SetTransform(T);
        }
        //break;
    }

    // Init Light
    scene.mLights.push_back(&mLight);
    game.mRenderer.SetScene(&scene);
    game.mInputHandler.AddEventListener(this);
}

void PBRDemo::KeyPress(const SDL_KeyboardEvent &e)
{
}

void PBRDemo::MouseButton(const SDL_MouseButtonEvent &e)
{
}

void PBRDemo::MouseMotion(const SDL_MouseMotionEvent &e)
{
}
