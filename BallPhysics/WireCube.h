/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef BP_WIRECUBE_H
#define BP_WIRECUBE_H

#include <Engine/Engine/Renderable.h>

class WireCube : public engine::Renderable
{
public:
    WireCube(float cubeSize);
    ~WireCube(void);

    virtual void Draw(const engine::RenderingContext &rContext) const override;

private:
    GLsizei mCount;
    GLuint mVBO;
    engine::Program mProgram;
};

#endif
