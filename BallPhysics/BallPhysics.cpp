/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "BallPhysics.h"
#include <assert.h>
#include <vector>
#include <random>
#include <easylogging++.h>

using namespace engine;
using namespace glm;
using namespace std;

BallPhysics::BallPhysics(const SimulationParams &simParams)
    : mPaused(false), mSolver(nullptr), mCtBalls(simParams.mCtBalls), mBallRadius(simParams.mBallRadius), mCubeSize(simParams.mCubeSize)
{
}

std::vector<glm::vec3> BallPhysics::GetBallPositions() const
{
    const auto &balls = mSolver->GetBalls();
    vector<glm::vec3> positions(mCtBalls);
    for(int i = 0; i < mCtBalls; ++i)
        positions[i] = balls[i].mPosition;
    return positions;
}


const SimulationParams& BallPhysics::GetSimParams() const
{
    return mSolver->GetSimParams();
}

void BallPhysics::SetSolver(PhysicsSolver *solver)
{
    if(mSolver == nullptr)
        mSolver = solver;
    else
    {
        solver->SetBalls(mSolver->GetBalls());
        mSolver = solver;
    }
}

void BallPhysics::Update(float dt)
{
    if(!mPaused)
        mSolver->Solve(dt);
}


void BallPhysics::InitBalls()
{
    const int ballsPerRow = int(mCubeSize / mBallRadius) - 1;

    if(ballsPerRow * ballsPerRow * ballsPerRow < mCtBalls)
    {
        LOG(ERROR) << "Balls dont fint into the box.";
        std::exit(-1);
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(-mBallRadius * 0.05f, mBallRadius * 0.05f);

    vector<Ball> balls;
    int ctBalls = 0;
    for(int i = 0; i < ballsPerRow; ++i)
    {
        const float x = float(i * 2 + 2) * mBallRadius - mCubeSize;
        for(int j = 0; j < ballsPerRow; ++j)
        {
            const float z = float(j * 2 + 2) * mBallRadius - mCubeSize;
            for(int k = 0; k < ballsPerRow; ++k)
            {
                if(ctBalls == mCtBalls)
                    break;

                const float y = float(k * 2 + 2) * mBallRadius - mCubeSize;
                balls.push_back(Ball(vec3(y + dis(gen), x + dis(gen), z + dis(gen))));
                ++ctBalls;
            }
            if(ctBalls == mCtBalls)
                break;
        }
        if(ctBalls == mCtBalls)
            break;
    }

    mSolver->SetBalls(balls);
}

void BallPhysics::Pause(bool pause)
{
    mPaused = pause;
}

