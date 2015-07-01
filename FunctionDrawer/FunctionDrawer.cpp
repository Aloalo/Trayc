#include "FunctionDrawer.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
using namespace engine;

FunctionDrawer::FunctionDrawer(const char *progFile)
    : mVertexShader(progFile), mFileName(progFile)
{
    // load the fragment shader source
    string sn(progFile);
    sn += ".frag";

    ifstream in(sn, ifstream::in);
    if(in)
    {
        stringstream strStream;
        strStream << in.rdbuf();
        mFragSource = strStream.str();
        mFragSource.erase(mFragSource.find_last_of('}')+1, mFragSource.length());
    }
    else
    {
        cerr << "No shader file found: " << sn << endl;
    }
    in.close();
}

FunctionDrawer::~FunctionDrawer(void)
{
    mProgram.Delete();
}

void FunctionDrawer::SetFunction(const string &F, const string &Fx, const string &Fy)
{
    this->mF = F;
    this->mFx = Fx;
    this->mFy = Fy;
}

const AABB& FunctionDrawer::GetAABB() const
{
    return mBox;
}

void FunctionDrawer::SetAABB(const AABB &box)
{
    this->mBox = box;
}
