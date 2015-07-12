/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef BP_UNFIRMGRID_H
#define BP_UNFIRMGRID_H

#include "PhysicsSolver.h"
#include <array>

template<class T, int N>
struct tcell
{
    tcell(void)
    {
        memset(mValues, -1, sizeof(T) * N);
    }

    static inline int GetCapacity()
    {
        return N;
    }

    inline const T* begin() const
    {
        return &mValues[0];
    }

    inline const T* end() const
    {
        return &mValues[N];
    }

    inline T* begin()
    {
        return &mValues[0];
    }

    inline T* end()
    {
        return &mValues[N];
    }

    T mValues[N];
};

using Cell = tcell<int, 4>;

//Uniform grid data structure
class UniformGrid
{
public:
    UniformGrid(float cubeSize, float ballRadius);

    int AddToGrid(const glm::vec3 &pos, int idx);
    int Update(const glm::vec3 &pos, int gridIdx, int ballIdx);
    std::vector<int> GetNeighbours(int gridIdx) const;

private:
    int mGridDim;
    int mGridDimSq;
    int mMaxNeighbours;
    float mCubeSize;
    float mBallRadius;
    float mInvBallDiameter;
    std::vector<Cell> mCells;
};

#endif
