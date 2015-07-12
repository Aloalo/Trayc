/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef BP_UNIFORMGRIDSOLVER_H
#define BP_UNIFORMGRIDSOLVER_H

#include "PhysicsSolver.h"
#include "UniformGrid.h"

//O(N) * big_constant solver
class UniformGridSolver : public PhysicsSolver
{
public:
    UniformGridSolver(const SimulationParams &simParams);

private:
    virtual void CollisionDetection() override;
    void CollisionDetectionRange(int first, int last);
    virtual void Init();

    UniformGrid mUnformGrid;
    //Ball i has Uniform grid index mCellIndices[i]
    std::vector<int> mCellIndices;
};

#endif
