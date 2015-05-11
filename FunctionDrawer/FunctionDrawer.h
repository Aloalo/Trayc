/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef FD_FUNCTIONDRAWER_H
#define FD_FUNCTIONDRAWER_H

#include <Engine/GL/Program.h>
#include <Engine/Core/Camera.h>

class FunctionDrawer
{
public:
    FunctionDrawer(const char *progFile);
    virtual ~FunctionDrawer(void);

    virtual void SetFunction(const std::string &F, const std::string &Fx, const std::string &Fy) = 0;
    virtual void ApplyFunction() = 0;
    
    virtual void Draw(const engine::Camera &cam) = 0;

protected:
    std::string F;
    std::string Fx;
    std::string Fy;

    // contains the fragment shader source
    std::string fragSource;
    std::string fileName;
    engine::VertexShader vs;
    engine::Program p;
};

#endif
