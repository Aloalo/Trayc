;/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "UniformGridSolver.h"

using namespace glm;
using namespace std;

UniformGridSolver::UniformGridSolver(const SimulationParams &simParams)
    : PhysicsSolver(simParams), mUnformGrid(simParams.mCubeSize, simParams.mBallRadius),
    mCellIndices(simParams.mCtBalls, -1)
{
}

void UniformGridSolver::CollisionDetection()
{
    const int ctBalls = mSimParams.mCtBalls;

    //Update grid
    for(int i = 0; i < ctBalls; ++i)
    {
        const int newGridIdx = mUnformGrid.Update(mBalls[i].mPosition, mCellIndices[i], i);
        mCellIndices[i] = newGridIdx;
    }

    //Collision detection
    for(int i = 0; i < ctBalls; ++i)
    {
        Ball A = mBalls[i];
        const int gridIdx = mCellIndices[i];

        for(int neighbourIdx : mUnformGrid.GetNeighbours(gridIdx))
        {
            if(neighbourIdx != i)
            {
                Ball &B = mBalls[neighbourIdx];
                const vec3 force_ij = CollideBalls(A, B);
                A.mVelocity += force_ij;
                B.mVelocity -= force_ij;
            }
        }
        mBalls[i] = A;
    }
}

void UniformGridSolver::Init()
{
    const int ctBalls = mSimParams.mCtBalls;
    for(int i = 0; i < ctBalls; ++i)
        mCellIndices[i] = mUnformGrid.AddToGrid(mBalls[i].mPosition, i);
}
