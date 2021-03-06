#include "PBRMaterialDemo.h"
#include <Engine/Geometry/GeometryMeshes.h>
#include <Engine/Engine/AssetLoader.h>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>


using namespace engine;
using namespace glm;
using namespace std;

PBRMaterialDemo::PBRMaterialDemo(void)
    : mMouseDown(false), mCurrObjIdx(0), mRotX(0.0f), mRotY(0.0f), 
    mLight(vec3(0.0f), vec3(1.0f), true, vec3(0.0f, 1.0f, 1.0f))
{
}

void PBRMaterialDemo::Init(RasterizedGame &game, Scene &scene)
{
    const string path = AssetLoader::Get().TexturePath("pbr/");
    ifstream in(path + "list.txt");

#ifdef _DEBUG
    const int sphereQuality = 3;
#else
    const int sphereQuality = 5;
#endif

    TriangleMesh sphere = GetSphereMeshSolid(false, sphereQuality, 50.0f);
    scene.mTriMeshes.push_back(sphere);
    const int sphereMeshIdx = static_cast<int>(scene.mTriMeshes.size()) - 1;

    scene.mObjects3D.reserve(50);
    string line;
    while(getline(in, line))
    {
        Material mat;
        mat.mGloss = 1.0f;
        mat.mKs = vec3(1.0f);
        mat.mKd = vec3(1.0f);
        mat.mNeedsForwardRender = false;
        string texPath = path + line + "/" + line;
        mat.mTextureMaps.push_back(Material::TextureInfo(texPath + "-albedo.png", TextureType::DIFFUSE_MAP));
        mat.mTextureMaps.push_back(Material::TextureInfo(texPath + "-normal.png", TextureType::NORMAL_MAP));
        mat.mTextureMaps.push_back(Material::TextureInfo(texPath + "-metal-rough.png", TextureType::SPECULAR_MAP));

        scene.mMaterials.push_back(mat);
        const int matIdx = static_cast<int>(scene.mMaterials.size()) - 1;

        scene.AddObject(AssetLoader::Get().CreateObject(&scene, sphereMeshIdx, matIdx));
        mObjects.push_back(&scene.mObjects3D[scene.mObjects3D.size() - 1]);
        mObjects.back()->mShadowCaster = false;
        mObjects.back()->mVisible = false;
    }

    /*for(float rough = 0.0f; rough < 1.0f; rough += 0.2f) {
    for(float metal = 0.0f; metal < 1.0f; metal += 0.2f) {
    Material mat;
    mat.mGloss = rough;
    mat.mKs = vec3(metal);
    mat.mKd = vec3(0.5f);
    mat.mNeedsForwardRender = false;
    scene.mMaterials.push_back(mat);
    const int matIdx = scene.mMaterials.size() - 1;
    scene.AddObject(AssetLoader::Get().CreateObject(&scene, sphereMeshIdx, matIdx));
    mObjects.push_back(&scene.mObjects3D[scene.mObjects3D.size() - 1]);
    mObjects.back()->mShadowCaster = false;
    mObjects.back()->mVisible = false;
    }
    }*/

    /*for(float x = 0.0f; x <= 1.0f; x += 0.2f) {
    Material mat;
    mat.mGloss = 0.8;
    mat.mKs = vec3(x);
    mat.mKd = vec3(0.5f);
    mat.mNeedsForwardRender = false;
    scene.mMaterials.push_back(mat);
    const int matIdx = scene.mMaterials.size() - 1;
    scene.AddObject(AssetLoader::Get().CreateObject(&scene, sphereMeshIdx, matIdx));
    mObjects.push_back(&scene.mObjects3D[scene.mObjects3D.size() - 1]);
    mObjects.back()->mShadowCaster = false;
    mObjects.back()->mVisible = false;
    }*/


    mObjects[mCurrObjIdx]->mVisible = true;

    // Init Light
    scene.mLights.push_back(&mLight);
    game.mRenderer.SetScene(&scene);
    game.mInputHandler.AddEventListener(this);
}

void PBRMaterialDemo::KeyPress(const SDL_KeyboardEvent &e)
{
    if(e.repeat || e.type == SDL_KEYUP)
        return;

    if(e.keysym.sym == SDLK_LEFT) {
        mObjects[mCurrObjIdx]->mVisible = false;
        mCurrObjIdx = mCurrObjIdx == 0 ? static_cast<int>(mObjects.size()) - 1 : mCurrObjIdx - 1;
    }
    else if(e.keysym.sym == SDLK_RIGHT) {
        mObjects[mCurrObjIdx]->mVisible = false;
        mCurrObjIdx = (mCurrObjIdx + 1) % mObjects.size();
    }

    mObjects[mCurrObjIdx]->mVisible = true;
}

void PBRMaterialDemo::MouseButton(const SDL_MouseButtonEvent &e)
{
    if(e.button == SDL_BUTTON_RIGHT) {
        mMouseDown = (e.state == SDL_PRESSED);
    }
}

void PBRMaterialDemo::MouseMotion(const SDL_MouseMotionEvent &e)
{
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
