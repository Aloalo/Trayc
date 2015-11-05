/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "PhysicsSolver.h"
#include <vector>
#include <assert.h>

using namespace glm;
using namespace std;


PhysicsSolver::PhysicsSolver(const SimulationParams &simParams)
    : mParallel(true), mSimParams(simParams), mBallDiameter(2.0f * simParams.mBallRadius),
    mInterpenetrationEps(simParams.mBallRadius * 0.001f)
{
}

PhysicsSolver::~PhysicsSolver(void)
{
}

void PhysicsSolver::SetBalls(const vector<Ball> &balls)
{
    mBalls = balls;
    Init();
}

const std::vector<Ball>& PhysicsSolver::GetBalls() const
{
    return mBalls;
}

void PhysicsSolver::Solve(float dt)
{
    Integrate(dt);
    CollisionDetection();
}

void PhysicsSolver::Integrate(float dt)
{
    const float globalDamping = mSimParams.mGlobalDamping;
    const float cubeDamping = mSimParams.mBoundaryDamping;
    const float minPos = -mSimParams.mCubeSize + mSimParams.mBallRadius;
    const float maxPos = mSimParams.mCubeSize - mSimParams.mBallRadius;
    const VectorField *field = mSimParams.mField;

    for(Ball &ball : mBalls)
    {
        vec3 p(ball.mPosition);
        vec3 v(ball.mVelocity);

        v += dt * field->F(p);
        v *= globalDamping;
        p += dt * v;

        if(p.x > maxPos)
        {
            p.x = maxPos;
            v.x *= cubeDamping;
        }
        else if(p.x < minPos)
        {
            p.x = minPos;
            v.x *= cubeDamping;
        }

        if(p.y > maxPos)
        {
            p.y = maxPos;
            v.y *= cubeDamping;
        }
        else if(p.y < minPos)
        {
            p.y = minPos;
            v.y *= cubeDamping;
        }

        if(p.z > maxPos)
        {
            p.z = maxPos;
            v.z *= cubeDamping;
        }
        else if(p.z < minPos)
        {
            p.z = minPos;
            v.z *= cubeDamping;
        }

        ball.mPosition = p;
        ball.mVelocity = v;
    }
}

glm::vec3 PhysicsSolver::CollideBalls(Ball &A, Ball &B) const
{
    const vec3 relPos = B.mPosition - A.mPosition;
    const float dist = length(relPos);
    const float collideDist = mBallDiameter;

    vec3 force(0.0f);

    if(dist < collideDist)
    {
        const vec3 norm = relPos / dist;
        //Relative velocity
        const vec3 relVel = B.mVelocity - A.mVelocity;
        //Relative tangential velocity
        const vec3 tanVel = relVel - dot(relVel, norm) * norm;

        const float interpenetration = collideDist - dist;

        //Spring force
        force = -mSimParams.mSpring * interpenetration * norm;
        //Damping force
        force += mSimParams.mDamping * relVel;
        //Tangential shear force
        force += mSimParams.mShear * tanVel;
        //Attraction
        force += mSimParams.mAttraction * relPos;

        //Resolve interpenetration
        const vec3 relMove = norm * (0.5f * interpenetration + mInterpenetrationEps);
        A.mPosition -= relMove;
        B.mPosition += relMove;
    }

    return force;
}

const SimulationParams& PhysicsSolver::GetSimParams() const
{
    return mSimParams;
}
