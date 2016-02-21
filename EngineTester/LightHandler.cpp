
#include "LightHandler.h"
#include <Engine/Geometry/Scene.h>
#include <Engine/Geometry/GeometryMeshes.h>
#include <Engine/Geometry/Material.h>

#include <glm/gtc/matrix_transform.hpp>

using namespace engine;
using namespace glm;
using namespace std;

LightHandler::LightHandler(engine::Scene *scene) :
    mALight(vec3(0.05f), true),
    mDLight(vec3(0.9f), true, vec3(1.0f)),
    mPLight(vec3(0.8f), true, vec3(1.0f, 0.3f, 0.01f), vec3(0.0f, 2.0f, 0.0f)),
    mSLight(vec3(0.8f), true, vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 5.5f, 0.0f), vec3(1.0f, 0.0f, 0.0f), 15.0f, 200.0f)
{
    // Init Lights
    scene->mLights.push_back(&mALight);
    scene->mLights.push_back(&mDLight);
    scene->mLights.push_back(&mPLight);
    scene->mLights.push_back(&mSLight);

    // Init BSpline
    const vec3 &lPos = mPLight.GetPosition();
    vector<vec3> controlPoints;
    controlPoints.push_back(lPos + vec3(0.0f, 0.0f, 0.0f));
    controlPoints.push_back(lPos + vec3(3.0f, 0.0f, 0.0f));
    controlPoints.push_back(lPos + vec3(0.0f, 3.0f, 0.0f));
    controlPoints.push_back(lPos + vec3(0.0f, 0.0f, 2.0f));

    controlPoints.push_back(lPos + vec3(0.0f, 0.0f, 0.0f));
    controlPoints.push_back(lPos + vec3(3.0f, 0.0f, 0.0f));
    controlPoints.push_back(lPos + vec3(0.0f, 3.0f, 0.0f));
    mBSpline.SetControlPoints(controlPoints);

    // Init Light Rendering
    Material lightMat;
    lightMat.mGloss = 1.0f;
    lightMat.mKs = vec3(1.0f);
    lightMat.mKd = vec3(1.0f);
    scene->mMaterials.push_back(lightMat);
    const int lightMatIdx = scene->mMaterials.size() - 1;
    
    TriangleMesh lightMesh = GetSphereMeshSolid(true, 1);
    lightMesh.FlipNormals();
    scene->mTriMeshes.push_back(lightMesh);
    const int lightMeshIdx = scene->mTriMeshes.size() - 1;

    scene->mObjects3D.push_back(Object3D(lightMeshIdx, lightMatIdx));
    mPLightObj = &scene->mObjects3D.back();
    mPLightObj->mShadowCaster = false;
}

void LightHandler::Update(float dt)
{
    static float accumBSplineDT = 0.0f;
    accumBSplineDT += dt;

    const float maxDT = float(mBSpline.NumControlPoints() - 3);
    if(accumBSplineDT >= maxDT) {
        accumBSplineDT -= maxDT;
    }
    const vec3 newPLightPos = mBSpline[accumBSplineDT];
    mPLight.SetPosition(newPLightPos);
    mPLightObj->SetTransform(scale(translate(mat4(1.0f), newPLightPos), vec3(0.05f)));

    static float accumSLight = 0.0f;
    accumSLight += dt;

    mSLight.SetDirection(vec3(cosf(accumSLight), 0.0f, sinf(accumSLight)));
}
