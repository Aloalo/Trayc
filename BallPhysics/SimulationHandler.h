/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef BP_SIMUALTIONHANDLER_H
#define BP_SIMUALTIONHANDLER_H

#include <Engine/Core/InputObserver.h>
#include <Engine/Engine/Renderable.h>
#include "BallPhysics.h"
#include "BallRenderer.h"
#include "WireCube.h"

class SimulationHandler : public engine::InputObserver, public engine::Renderable
{
public:
    SimulationHandler(const SimulationParams &simParams, const RenderingParams &rParams);
    ~SimulationHandler(void);

    void Init(engine::Scene &scene);

private:
    virtual void KeyPress(const SDL_KeyboardEvent &e) override;
    virtual void Draw(const engine::RenderingContext &rContext) const override;

    //Solvers
    std::vector<PhysicsSolver*> mSolvers;

    bool mPhysicsPaused;
    SimulationParams mSimParams;
    BallPhysics mPhysics;

    RenderingParams mRenderingParams;
    BallRenderer mRenderer;
    WireCube mCube;
};

#endif
