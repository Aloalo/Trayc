/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "SimulationHandler.h"
#include "Solvers/SlowSolver.h"
#include <Engine/Utils/StlExtensions.hpp>
#include <glm/gtx/norm.hpp>

using namespace engine;
using namespace glm;
using namespace std;

SimulationHandler::SimulationHandler(const SimulationParams &simParams, const RenderingParams &rParams)
    : mPhysicsPaused(false), mSimParams(simParams), mPhysics(simParams), mRenderingParams(rParams), 
    mRenderer(rParams, simParams.mCtBalls, mSimParams.mCubeSize, mSimParams.mBallRadius), mCube(simParams.mCubeSize)
{
    mSolvers.push_back(new SlowSolver(mSimParams));
    mPhysics.SetSolver(mSolvers[0]);
}

SimulationHandler::~SimulationHandler(void)
{
    for(PhysicsSolver *solver : mSolvers)
    {
        delete solver;
    }
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
