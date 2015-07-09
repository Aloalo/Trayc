/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef BP_BALLHANDLER_H
#define BP_BALLHANDLER_H

#include <Engine/Engine/Renderable.h>
#include <Engine/Engine/Scene.h>
#include "BallPhysics.h"

class BallRenderer : public engine::Renderable
{
public:
    BallRenderer(const BallPhysics *mPhysics, int ctBalls, float cubeSize, float ballRadius, int windowHeight, float FOV, const glm::vec3 &lightDir);
    ~BallRenderer(void);

    //Copies memory to GPU, expensive
    void SetBalls(const glm::vec3 *balls) const;
    virtual void Draw(const engine::RenderingContext &rContext) const override;

private:
    const int mCtBalls;
    const BallPhysics *mPhysics;
    GLuint mVBO;
    engine::Program mProgram;
};

#endif
