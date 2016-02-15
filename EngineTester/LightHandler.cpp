
#include "LightHandler.h"
#include <Engine/Geometry/Scene.h>
#include <Engine/Geometry/GeometryMeshes.h>
#include <Engine/Geometry/Material.h>

#include <glm/gtc/matrix_transform.hpp>

using namespace engine;
using namespace glm;
using namespace std;

LightHandler::LightHandler(engine::Scene *scene) :
    mDLight(vec3(0.8f), true, vec3(1.0f)),
    mPLight(vec3(0.8f), true, vec3(1.0f, 0.3f, 0.01f), vec3(0.0f, 2.0f, 0.0f))
{
    // Init Lights
    scene->mLights.push_back(&mDLight);
    scene->mLights.push_back(&mPLight);

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
    static float accum = 0.0f;
    accum += dt;

    const float maxDT = float(mBSpline.NumControlPoints() - 3);
    if(accum >= maxDT) {
        accum -= maxDT;
    }

    const vec3 newPLightPos = mBSpline[accum];
    mPLight.SetPosition(newPLightPos);

    mPLightObj->SetTransform(scale(translate(mat4(1.0f), newPLightPos), vec3(0.05f)));
}
