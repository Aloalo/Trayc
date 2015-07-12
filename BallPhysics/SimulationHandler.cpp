/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "SimulationHandler.h"
#include "Solvers/SlowSolver.h"
#include "Solvers/UniformGridSolver.h"
#include <Engine/Utils/StlExtensions.hpp>
#include <glm/gtx/norm.hpp>

using namespace engine;
using namespace glm;
using namespace std;

SimulationHandler::SimulationHandler(const SimulationParams &simParams, const RenderingParams &rParams)
    : mPhysicsPaused(false), mPhysics(simParams), mRenderingParams(rParams), 
    mRenderer(rParams, simParams.mCtBalls, simParams.mCubeSize, simParams.mBallRadius), mCube(simParams.mCubeSize)
{
    mFields.push_back(new Gravity());
    mFields.push_back(new SphereAttractor());
    mFields.push_back(new SphereRepulsor());
    mFields.push_back(new SphereRotator());
    mFields.push_back(new CameraGravity(rParams.mCamera));
    mCurrentField = 0;

    mSolvers.push_back(new SlowSolver(simParams));
    mSolvers.push_back(new UniformGridSolver(simParams));
    mCurrentSolver = 1;

    for(auto solver : mSolvers)
        solver->mSimParams.mField = mFields[mCurrentField];

    mPhysics.SetSolver(mSolvers[mCurrentSolver]);
}

SimulationHandler::~SimulationHandler(void)
{
    for(auto solver : mSolvers)
        delete solver;
    for(auto field : mFields)
        delete field;
}

void SimulationHandler::KeyPress(const SDL_KeyboardEvent &e)
{
    if(e.type == SDL_KEYDOWN)
    {
        switch(e.keysym.sym)
        {
        case SDLK_p:
            mPhysicsPaused = !mPhysicsPaused;
            mPhysics.Pause(mPhysicsPaused);
            break;
        case SDLK_s:
            mCurrentSolver = (mCurrentSolver + 1) % mFields.size();
            mPhysics.SetSolver(mSolvers[mCurrentSolver]);
            break;
        case SDLK_f:
            mCurrentField = (mCurrentField + 1) % mFields.size();
            for(auto solver : mSolvers)
                solver->mSimParams.mField = mFields[mCurrentField];
            break;
        case SDLK_c:
            for(auto solver : mSolvers)
                solver->mParallel = !solver->mParallel;
            break;
        default:
            break;
        }
    }
}

void SimulationHandler::Init(Scene &scene)
{
    mPhysics.InitBalls();
    scene.mUpdateableMenager.AddUpdateable(&mPhysics);
    scene.mRenderer.AddLight(mRenderingParams.mLight);
    scene.mRenderer.AddRenderable(this);
    scene.mInputHandler.AddEventListener(this);

    scene.mRenderer.AddRenderable(&mCube);
}

void SimulationHandler::Draw(const RenderingContext &rContext) const
{
    mRenderer.SetBalls(mPhysics.GetBallPositions().data());
    mRenderer.Draw(rContext);
}
