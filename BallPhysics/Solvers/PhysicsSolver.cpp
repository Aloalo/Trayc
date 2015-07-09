/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "PhysicsSolver.h"
#include <vector>

using namespace glm;
using namespace std;


PhysicsSolver::PhysicsSolver(float cubeSize, float ballRadius)
    : mCubeSize(cubeSize), mBallRadius(ballRadius), mField(nullptr)
{
}

void PhysicsSolver::SetVectorField(const VectorField *field)
{
    mField = field;
}

void PhysicsSolver::SetBalls(const vector<Ball> &balls)
{
    mBalls = balls;
}

const std::vector<Ball>& PhysicsSolver::GetBalls() const
{
    return mBalls;
}

void PhysicsSolver::CollideCube()
{
    const float cubeSize = mCubeSize;
    const float ballRadius = mBallRadius;
    for(Ball &ball : mBalls)
    {
        const vec3 p(ball.mPosition);
        if(p.x - ballRadius < -cubeSize || p.x + ballRadius > cubeSize)
        {
            ball.mVelocity.x -= 2.0f * ball.mVelocity.x;
        }
        if(p.y - ballRadius < -cubeSize || p.y + ballRadius > cubeSize)
        {
            ball.mVelocity.y -= 2.0f * ball.mVelocity.y;
        }
        if(p.z - ballRadius < -cubeSize || p.z + ballRadius > cubeSize)
        {
            ball.mVelocity.z -= 2.0f * ball.mVelocity.z;
        }
    }
}
