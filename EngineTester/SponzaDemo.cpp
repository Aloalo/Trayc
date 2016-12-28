
#include "SponzaDemo.h"
#include <Engine/Engine/AssetLoader.h>
#include <Engine/Geometry/Scene.h>
#include <Engine/Geometry/GeometryMeshes.h>
#include <Engine/Geometry/Material.h>

#include <glm/gtc/matrix_transform.hpp>

using namespace engine;
using namespace glm;
using namespace std;

SponzaDemo::SponzaDemo(void) :
    mPause(false),
    mGLight(vec3(0.001f), 5.0f * vec3(0.988f, 0.83f, 0.251f), true, vec3(1.0f)),
    mPLight(vec3(20.0f), true, vec3(1.0f, 0.1f, 0.01f), vec3(0.0f, 200.0f, 0.0f)), mPLightObj(nullptr),
    mSLight(vec3(1.0f), true, vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 600.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), 15.0f, 200.0f)
{
}

void SponzaDemo::Init(Scene *scene)
{
    // Init Lights
    scene->mLights.push_back(&mGLight);
    scene->mLights.push_back(&mPLight);
    scene->mLights.push_back(&mSLight);

    // Init BSpline
    const vec3 &lPos = mPLight.GetPosition();
    const float offs = 300.0f;
    vector<vec3> controlPoints;
    controlPoints.push_back(lPos + vec3(0.0f, 0.0f, 0.0f));
    controlPoints.push_back(lPos + vec3(offs, 0.0f, 0.0f));
    controlPoints.push_back(lPos + vec3(0.0f, offs, 0.0f));
    controlPoints.push_back(lPos + vec3(0.0f, 0.0f, 200.0f));

    controlPoints.push_back(lPos + vec3(0.0f, 0.0f, 0.0f));
    controlPoints.push_back(lPos + vec3(offs, 0.0f, 0.0f));
    controlPoints.push_back(lPos + vec3(0.0f, offs, 0.0f));
    mBSpline.SetControlPoints(controlPoints);

    // Init Light Rendering
    Material lightMat;
    lightMat.mGloss = 1.0f;
    lightMat.mKs = vec3(1.0f);
    lightMat.mKd = vec3(1.0f);
    lightMat.mNeedsForwardRender = true;
    scene->mMaterials.push_back(lightMat);
    const int lightMatIdx = scene->mMaterials.size() - 1;

    TriangleMesh lightMesh = GetSphereMeshSolid(false, 1, 5.0f);
    lightMesh.FlipNormals();
    scene->mTriMeshes.push_back(lightMesh);
    const int lightMeshIdx = scene->mTriMeshes.size() - 1;

    scene->AddObject(AssetLoader::Get().CreateObject(scene, lightMeshIdx, lightMatIdx));
    mPLightObj = &scene->GetObject(scene->GetNumObjects() - 1);
    mPLightObj->mShadowCaster = false;
}

void SponzaDemo::Update(float dt)
{
    if(mPause) {
        return;
    }

    static float accumBSplineDT = 0.0f;
    accumBSplineDT += dt;

    const float maxDT = float(mBSpline.NumControlPoints() - 3);
    if(accumBSplineDT >= maxDT) {
        accumBSplineDT -= maxDT;
    }
    const vec3 newPLightPos = mBSpline[accumBSplineDT];
    mPLight.SetPosition(newPLightPos);
    mPLightObj->SetTransform(translate(mat4(1.0f), newPLightPos));

    static float accumSLight = 0.0f;
    accumSLight += dt;

    mSLight.SetDirection(vec3(cosf(accumSLight), 0.0f, sinf(accumSLight)));

    const vec3 d = vec3(0.0f, abs(sinf(accumSLight / 5.0f)), cosf(accumSLight / 5.0f)) / 3.0f;
    mGLight.SetDirection(normalize(d));
}

void SponzaDemo::KeyPress(const SDL_KeyboardEvent & e)
{
    if(e.repeat) {
        return;
    }

    if(e.keysym.sym == SDLK_p && e.type == SDL_KEYDOWN) {
        mPause = !mPause;
    }
}
