/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef BP_SLOWSOLVER_H
#define BP_SLOWSOLVER_H

#include "PhysicsSolver.h"

//O(N^2) solver
class SlowSolver : public PhysicsSolver
{
public:
    SlowSolver(float cubeSize, float ballRadius);

    virtual void Solve(float dt) override;
};

#endif
