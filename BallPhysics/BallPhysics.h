/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef BP_BALLPHYSICS_H
#define BP_BALLPHYSICS_H

#include <Engine/Engine/Scene.h>
#include "Solvers/PhysicsSolver.h"

class BallPhysics : public engine::Updateable
{
public:
    BallPhysics(PhysicsSolver *solver, int ctBalls, float cubeSize, float ballRadius);

    void InitBalls();
    std::vector<glm::vec3> GetBallPositions() const;
    virtual void Update(float dt) override;

private:
    PhysicsSolver *mSolver;
    const int mCtBalls;
    const float mBallRadius;
    const float mCubeSize;
};

#endif
