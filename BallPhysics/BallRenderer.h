/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef BP_BALLRENDERER_H
#define BP_BALLRENDERER_H

#include <Engine/Engine/Scene.h>

struct RenderingParams
{
    //Screen size
    glm::ivec2 mSSize;
    float mFOV;
    bool mVsync;
    engine::Light mLight;
    const engine::Camera *mCamera;
};

class BallRenderer : public engine::Renderable
{
public:
    BallRenderer(const RenderingParams &rParams, int ctBalls, float cubeSize, float ballRadius);
    ~BallRenderer(void);

    //Copies memory to GPU, expensive
    void SetBalls(const glm::vec3 *balls) const;
    virtual void Draw(const engine::RenderingContext &rContext) const override;

private:
    const int mCtBalls;
    GLuint mVBO;
    engine::Program mProgram;
};

#endif
