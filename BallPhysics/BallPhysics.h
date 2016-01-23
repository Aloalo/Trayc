/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef BP_BALLPHYSICS_H
#define BP_BALLPHYSICS_H

#include <Engine/Engine/Game.h>
#include "Solvers/PhysicsSolver.h"

class BallPhysics : public engine::Updateable
{
public:
    BallPhysics(const SimulationParams &simParams);

    std::vector<glm::vec3> GetBallPositions() const;
    const SimulationParams& GetSimParams() const;

private:
    friend class SimulationHandler;

    virtual void Update(float dt) override;
    void InitBalls();
    void SetSolver(PhysicsSolver *solver);
    void Pause(bool pause);

    bool mPaused;
    PhysicsSolver *mSolver;
    const int mCtBalls;
    const float mBallRadius;
    const float mCubeSize;
};

#endif
