/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "UniformGrid.h"
#include <iostream>
#include <assert.h>
#include <glm/gtx/norm.hpp>

using namespace glm;
using namespace std;

UniformGrid::UniformGrid(float cubeSize, float ballRadius)
    : mGridDim(int(cubeSize / ballRadius)), mGridDimSq(mGridDim * mGridDim), 
    mMaxNeighbours(Cell::GetCapacity() * Cell::GetCapacity() * Cell::GetCapacity()),
    mCubeSize(cubeSize), mBallRadius(ballRadius),
    mInvBallDiameter(1.0f / (2.0f * ballRadius)), mCells(mGridDim * mGridDim * mGridDim)
{
    const float gridDimF = mCubeSize / mBallRadius;
    if(fabs(gridDimF - float(mGridDim)) > 0.00001f)
    {
        cerr << "UnformGrid error: mCubeSize must be a multiple of mBallRadius." << endl;
        std::exit(-1);
    }
}


int UniformGrid::Update(const glm::vec3 &pos, int gridIdx, int ballIdx)
{
    const int i = int((pos.x + mCubeSize) * mInvBallDiameter);
    const int j = int((pos.y + mCubeSize) * mInvBallDiameter);
    const int k = int((pos.z + mCubeSize) * mInvBallDiameter);
    const int newGridIdx = i * mGridDimSq + j * mGridDim + k;

    if(newGridIdx == gridIdx)
        return newGridIdx;

    Cell &cell = mCells[gridIdx];
    for(auto &bIdx : cell)
    {
        if(bIdx == ballIdx)
        {
            bIdx = -1;
            break;
        }
    }

    Cell &newCell = mCells[newGridIdx];
    for(auto &bIdx : newCell)
    {
        if(bIdx == -1)
        {
            bIdx = ballIdx;
            break;
        }
    }

    return newGridIdx;
}


int UniformGrid::AddToGrid(const glm::vec3 &pos, int idx)
{
    const int i = int((pos.x + mCubeSize) * mInvBallDiameter);
    const int j = int((pos.y + mCubeSize) * mInvBallDiameter);
    const int k = int((pos.z + mCubeSize) * mInvBallDiameter);
    const int gridIdx = i * mGridDimSq + j * mGridDim + k;

    Cell &c = mCells[gridIdx];

    for(auto &ballIdx : c)
    {
        if(ballIdx == -1)
        {
            ballIdx = idx;
            break;
        }
    }
    //assert(ok);

    return gridIdx;
}

vector<int> UniformGrid::GetNeighbours(int gridIdx) const
{
    const int gridDim = mGridDim;
    const int gridDimSq = mGridDimSq;
    const int i = gridIdx / gridDimSq;
    const int j = (gridIdx - i * gridDimSq) / gridDim;
    const int k = (gridIdx - i * gridDimSq - j * gridDim);

    vector<int> neighbours;
    neighbours.reserve(mMaxNeighbours);
    for(int x = -1; x <= 1; ++x)
    {
        const int di = i + x;
        if(di >= 0 && di < gridDim)
        {
            for(int y = -1; y <= 1; ++y)
            {
                const int dj = j + y;
                if(dj >= 0 && dj < gridDim)
                {
                    for(int z = -1; z <= 1; ++z)
                    {
                        const int dk = k + z;
                        if(dk >= 0 && dk < gridDim)
                        {
                            const Cell &c = mCells[di * gridDimSq + dj * gridDim + dk];
                            for(auto ballIdx : c)
                                if(ballIdx != -1)
                                    neighbours.push_back(ballIdx);
                        }
                    }
                }
            }
        }
    }

    return neighbours;
}
