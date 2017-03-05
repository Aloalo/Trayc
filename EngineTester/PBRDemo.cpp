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
    : mMouseDown(false), mCurrObjIdx(0), mRotX(0.0f), mRotY(0.0f), 
    mLight(vec3(0.0f), vec3(1.0f), true, vec3(0.0f, 1.0f, 1.0f))
{
}

void PBRDemo::Init(Game &game, Scene &scene)
{
#ifdef _DEBUG
    const int sphereQuality = 3;
#else
    const int sphereQuality = 5;
#endif

    const float radius = 10.0f;
    TriangleMesh sphere = GetSphereMeshSolid(false, sphereQuality, radius);
    //TriangleMesh sphere = GetCubeMeshSolid(false, true, scale(mat4(1.0f), vec3(radius)));
    scene.mTriMeshes.push_back(sphere);
    const int sphereMeshIdx = scene.mTriMeshes.size() - 1;

    const float spacing = 2.5f * radius;
    const int ctBalls = 10;
    const float step = 1.0f / float(ctBalls);
    int i = 0;
    for(float rough = 0.1f; i < ctBalls; rough += step, ++i) {
        int j = 0;
        for(float metal = 0.1f; j < ctBalls; metal += step, ++j) {
            Material mat;
            mat.mGloss = rough;
            mat.mKs = vec3(metal);
            mat.mKd = vec3(0.0f);
            mat.mNeedsForwardRender = false;
            scene.mMaterials.push_back(mat);
            const int matIdx = scene.mMaterials.size() - 1;
            scene.AddObject(AssetLoader::Get().CreateObject(&scene, sphereMeshIdx, matIdx));
            mObjects.push_back(&scene.mObjects3D[scene.mObjects3D.size() - 1]);
            auto object = mObjects.back();
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
    return;

    if(e.repeat || e.type == SDL_KEYUP)
        return;

    if(e.keysym.sym == SDLK_LEFT) {
        mObjects[mCurrObjIdx]->mVisible = false;
        mCurrObjIdx = mCurrObjIdx == 0 ? mObjects.size() - 1 : mCurrObjIdx - 1;
    }
    else if(e.keysym.sym == SDLK_RIGHT) {
        mObjects[mCurrObjIdx]->mVisible = false;
        mCurrObjIdx = (mCurrObjIdx + 1) % mObjects.size();
    }

    mObjects[mCurrObjIdx]->mVisible = true;
}

void PBRDemo::MouseButton(const SDL_MouseButtonEvent &e)
{
    return;

    if(e.button == SDL_BUTTON_RIGHT) {
        mMouseDown = (e.state == SDL_PRESSED);
    }
}

void PBRDemo::MouseMotion(const SDL_MouseMotionEvent &e)
{
    return;

    if(mMouseDown) {
        static const mat4 I(1.0f);
        mRotX += float(e.xrel) / 80.0f;
        mRotY += float(e.yrel) / 80.0f;
        for(Object3D *obj : mObjects) {
            obj->SetTransform(rotate(I, mRotX, vec3(0.0f, 1.0f, 0.0f)));
            obj->SetTransform(obj->GetTransform() * rotate(I, mRotY, vec3(1.0f, 0.0f, 0.0f)));
        }
    }
}
