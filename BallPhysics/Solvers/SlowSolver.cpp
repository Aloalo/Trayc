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
        Ball A = mBalls[i];

        for(int j = i + 1; j < ctBalls; ++j)
        {
            Ball B = mBalls[j];
            const vec3 force_ij = CollideBalls(A, B);
            A.mVelocity += force_ij;
            B.mVelocity -= force_ij;

            mBalls[j] = B;
        }

        mBalls[i] = A;
    }

}
