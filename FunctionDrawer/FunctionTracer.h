/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef FD_FUNCTIONTRACER_H
#define FD_FUNCTIONTRACER_H

#include "FunctionDrawer.h"

class FunctionTracer : public FunctionDrawer
{
public:
    FunctionTracer(void);
    ~FunctionTracer(void);

    void SetFunction(const std::string &F, const std::string &Fx, const std::string &Fy);
    void ApplyFunction();

    virtual void Draw(const engine::Camera &cam) const override;

private:
    GLuint mVAO;
    GLuint mVBO;
};

#endif
