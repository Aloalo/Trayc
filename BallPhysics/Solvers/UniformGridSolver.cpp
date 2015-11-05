/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "UniformGridSolver.h"
#include <thread>
#include <cmath>

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

    if(mParallel)
    {
        //Parallel collision detection
        const int numThreads = thread::hardware_concurrency();
        const int ballsPerThread = (ctBalls % numThreads == 0 ? ctBalls / numThreads : ctBalls / numThreads + 1);
        vector<thread> threads;

        for(int i = 0; i < numThreads; ++i)
        {
            const int first = i * ballsPerThread;
            const int last = min((i + 1) * ballsPerThread, ctBalls);
            threads.push_back(thread(&UniformGridSolver::CollisionDetectionRange, this, first, last));
        }

        for(auto &t : threads)
            t.join();
    }
    else
    {
        //Non-parallel collision detection
        CollisionDetectionRange(0, ctBalls);
    }
}

void UniformGridSolver::Init()
{
    const int ctBalls = mSimParams.mCtBalls;
    for(int i = 0; i < ctBalls; ++i)
        mCellIndices[i] = mUnformGrid.AddToGrid(mBalls[i].mPosition, i);
}

void UniformGridSolver::CollisionDetectionRange(int first, int last)
{
    //Collision detection
    for(int i = first; i < last; ++i)
    {
        Ball A = mBalls[i];
        const int gridIdx = mCellIndices[i];

        for(int neighbourIdx : mUnformGrid.GetNeighbours(gridIdx))
        {
            if(neighbourIdx != i)
            {
                Ball B = mBalls[neighbourIdx];
                const vec3 force_ij = CollideBalls(A, B);
                A.mVelocity += force_ij;
                B.mVelocity -= force_ij;
                mBalls[neighbourIdx] = B;
            }
        }
        mBalls[i] = A;
    }
}
