#include "FunctionDrawer.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
using namespace engine;

FunctionDrawer::FunctionDrawer(const char *progFile)
    : vs(progFile), fileName(progFile)
{
    // load the fragment shader source
    string sn(progFile);
    sn += ".frag";

    ifstream in(sn, ifstream::in);
    if(in)
    {
        stringstream strStream;
        strStream << in.rdbuf();
        fragSource = strStream.str();
        fragSource.erase(fragSource.find_last_of('}')+1, fragSource.length());
    }
    else
    {
        cerr << "No shader file found: " << sn << endl;
    }
    in.close();
}

FunctionDrawer::~FunctionDrawer(void)
{
    p.Delete();
}

void FunctionDrawer::SetFunction(const string &F, const string &Fx, const string &Fy)
{
    this->F = F;
    this->Fx = Fx;
    this->Fy = Fy;
}

const AABB& FunctionDrawer::GetAABB() const
{
    return box;
}

void FunctionDrawer::SetAABB(const AABB &box)
{
    this->box = box;
}
