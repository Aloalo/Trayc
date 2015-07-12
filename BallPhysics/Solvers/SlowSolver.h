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
    SlowSolver(const SimulationParams &simParams);
    virtual void CollisionDetection() override;
    virtual void Init();
};

#endif
