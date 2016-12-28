#include "ShadowsDemo.h"
#include <Engine/Geometry/GeometryMeshes.h>
#include <Engine/Engine/AssetLoader.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


using namespace engine;
using namespace glm;
using namespace std;

ShadowsDemo::ShadowsDemo(void)
    : mLightLookAt(0.0f), mLightStartPos(100.0f), mRotX(0.0f), mCurrSceneIdx(0), mMouseDown(false),
    mLight(vec3(1.0f), true, vec3(1.0f, 0.005f, 0.0f), vec3(mLightStartPos), vec3(0.0f), 20.0f, 90.0f),
    mAmbientLight(vec3(0.075f), true)
{
    mLight.SetDirection(normalize(mLightLookAt - mLight.GetPosition()));
}

void ShadowsDemo::Init(Game &game, Scene &scene, const std::string &model)
{
    scene = AssetLoader::Get().LoadScene(AssetLoader::Get().ModelPath(model + "/"), model + ".obj");
    scene.Scale(80.0f);
    const float planeHeight = scene.GetAABB().mMinv.y;

    // init plane
    const TriangleMesh plane = GetPlane(1000.0f, planeHeight);
    Material mat;
    mat.mKd = vec3(1.0f);
    mat.mKs = vec3(0.1f);
    mat.mGloss = 0.0f;

    scene.mMaterials.push_back(mat);
    scene.mTriMeshes.push_back(plane);
    scene.AddObject(AssetLoader::Get().CreateObject(&scene, scene.mTriMeshes.size() - 1, scene.mMaterials.size() - 1));
    scene.mObjects3D.back().mShadowCaster = false;

    // Init light object
    Material lightMat;
    lightMat.mGloss = 1.0f;
    lightMat.mKs = vec3(1.0f);
    lightMat.mKd = vec3(1.0f);
    lightMat.mNeedsForwardRender = true;
    scene.mMaterials.push_back(lightMat);
    const int lightMatIdx = scene.mMaterials.size() - 1;

    TriangleMesh lightMesh = GetSphereMeshSolid(false, 1, 5.0f);
    lightMesh.FlipNormals();
    scene.mTriMeshes.push_back(lightMesh);
    const int lightMeshIdx = scene.mTriMeshes.size() - 1;

    scene.AddObject(AssetLoader::Get().CreateObject(&scene, lightMeshIdx, lightMatIdx));
    mPLightObj = &scene.GetObject(scene.GetNumObjects() - 1);
    mPLightObj->mShadowCaster = false;
    mPLightObj->SetTransform(translate(mat4(1.0f), mLight.GetPosition()));

    scene.mLights.push_back(&mAmbientLight);
    scene.mLights.push_back(&mLight);

    game.mRenderer.SetScene(&scene);
    game.mInputHandler.AddEventListener(this);

    mGame = &game;
}

void ShadowsDemo::MouseButton(const SDL_MouseButtonEvent &e)
{
    if(e.button == SDL_BUTTON_RIGHT) {
        mMouseDown = (e.state == SDL_PRESSED);
    }
}

void ShadowsDemo::MouseMotion(const SDL_MouseMotionEvent &e)
{
    if(mMouseDown) {
        static const mat4 I(1.0f);
        mRotX += float(e.xrel) / 400.0f;
        const mat3 R(rotate(I, mRotX, vec3(0.0f, 1.0f, 0.0f)));

        mLight.SetPosition((mLightStartPos - mLightLookAt) * R + mLightLookAt);
        mPLightObj->SetTransform(translate(I, mLight.GetPosition()));
        mLight.SetDirection(normalize(mLightLookAt - mLight.GetPosition()));
    }
}
