
#include "LightHandler.h"
#include <Engine/Engine/Game.h>

using namespace engine;
using namespace glm;
using namespace std;

LightHandler::LightHandler(engine::Game *game) :
    mDLight(vec3(0.8f), true, vec3(1.0f)),
    mPLight(vec3(0.8f), true, vec3(1.0f, 0.3f, 0.01f), vec3(0.0f, 2.0f, 0.0f))
{
    game->mRenderer.AddLight(&mDLight);
    game->mRenderer.AddLight(&mPLight);
    game->mUpdateableMenager.AddUpdateable(this);

    vector<vec3> controlPoints;
    controlPoints.push_back(mPLight.mPosition + vec3(0.0f, 0.0f, 0.0f));
    controlPoints.push_back(mPLight.mPosition + vec3(3.0f, 0.0f, 0.0f));
    controlPoints.push_back(mPLight.mPosition + vec3(0.0f, 3.0f, 0.0f));
    controlPoints.push_back(mPLight.mPosition + vec3(0.0f, 0.0f, 3.0f));

    controlPoints.push_back(mPLight.mPosition + vec3(0.0f, 0.0f, 0.0f));
    controlPoints.push_back(mPLight.mPosition + vec3(3.0f, 0.0f, 0.0f));
    controlPoints.push_back(mPLight.mPosition + vec3(0.0f, 3.0f, 0.0f));
    mBSpline.SetControlPoints(controlPoints);
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
    mPLight.mPosition = newPLightPos;
}
