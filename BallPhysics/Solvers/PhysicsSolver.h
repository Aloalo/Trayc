/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef BP_PHYSICSSOLVER_H
#define BP_PHYSICSSOLVER_H

#include "VectorFields.h"
#include "../Ball.h"
#include <vector>

struct SimulationParams
{
    //vec3 colliderPos;
    //float  colliderRadius;

    const VectorField *mField;
    int mCtBalls;

    float mBallRadius;
    float mCubeSize;
    float mGlobalDamping;
    float mSpring;
    float mDamping;
    float mShear;
    float mAttraction;
    float mBoundaryDamping;
};

class PhysicsSolver
{
public:
    PhysicsSolver(const SimulationParams &simParams);

    void Solve(float dt);
    void SetBalls(const std::vector<Ball> &balls);
    const std::vector<Ball>& GetBalls() const;
    const SimulationParams& GetSimParams() const;

protected:
    //Integrates simulation and collides with cube
    void Integrate(float dt);
    //Collide two balls
    glm::vec3 CollideBalls(const Ball &A, const Ball &B) const;

    virtual void CollisionDetection() = 0;

    SimulationParams mSimParams;

    std::vector<Ball> mBalls;
};

#endif
