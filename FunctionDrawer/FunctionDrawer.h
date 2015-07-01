/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef FD_FUNCTIONDRAWER_H
#define FD_FUNCTIONDRAWER_H

#include <Engine/GL/Program.h>
#include <Engine/Core/Camera.h>
#include <Engine/Geometry/AABB.h>

class FunctionDrawer
{
public:
    FunctionDrawer(const char *progFile);
    virtual ~FunctionDrawer(void);

    virtual void SetFunction(const std::string &F, const std::string &Fx, const std::string &Fy) = 0;
    virtual void ApplyFunction() = 0;
    
    virtual void Draw(const engine::Camera &cam) = 0;

    const engine::AABB& GetAABB() const;
    void SetAABB(const engine::AABB &box);

protected:
    engine::AABB mBox;

    std::string mF;
    std::string mFx;
    std::string mFy;

    // contains the fragment shader source
    std::string mFragSource;
    engine::VertexShader mVertexShader;
    std::string mFileName;
    engine::Program mProgram;
};

#endif
