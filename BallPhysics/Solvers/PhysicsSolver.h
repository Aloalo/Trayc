/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef BP_PHYSICSSOLVER_H
#define BP_PHYSICSSOLVER_H

#include "VectorFields.h"
#include "../Ball.h"
#include <vector>

class PhysicsSolver
{
public:
    PhysicsSolver(float cubeSize, float ballRadius);

    virtual void Solve(float dt) = 0;
    void SetVectorField(const VectorField *field);
    void SetBalls(const std::vector<Ball> &balls);
    const std::vector<Ball>& GetBalls() const;

protected:
    void CollideCube();

    float mCubeSize;
    float mBallRadius;
    const VectorField *mField;
    std::vector<Ball> mBalls;
};

#endif
