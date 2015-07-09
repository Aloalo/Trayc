/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "SlowSolver.h"
#include <vector>

using namespace glm;
using namespace std;

SlowSolver::SlowSolver(float cubeSize, float ballRadius)
    : PhysicsSolver(cubeSize, ballRadius)
{
}

void SlowSolver::Solve(float dt)
{
    const int ctBalls = mBalls.size();
    for(Ball &b : mBalls)
    {
        b.mVelocity += dt * mField->F(b.mPosition);
        b.mPosition += dt * b.mVelocity;
    }

    CollideCube();
}
