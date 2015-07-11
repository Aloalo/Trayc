/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "SlowSolver.h"
#include <vector>

using namespace glm;
using namespace std;

SlowSolver::SlowSolver(const SimulationParams &simParams)
    : PhysicsSolver(simParams)
{
}

void SlowSolver::CollisionDetection()
{
    const int ctBalls = mSimParams.mCtBalls;
    for(int i = 0; i < ctBalls; ++i)
    {
        vec3 force(0.0f);
        const Ball A = mBalls[i];

        for(int j = 0; j < ctBalls; ++j)
        {
            if(i != j)
            {
                const Ball B = mBalls[j];
                force += CollideBalls(A, B);
            }
        }

        mBalls[i].mVelocity += force;
    }
}
