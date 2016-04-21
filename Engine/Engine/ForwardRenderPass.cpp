/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Engine/ForwardRenderPass.h>

#include <easylogging++.h>

using namespace glm;
using namespace std;

namespace engine
{
    ForwardRenderPass::ForwardRenderPass(void)
        : RenderPass("forwardPass", GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, false),
        gPass(nullptr), lPass(nullptr)
    {
    }

    void ForwardRenderPass::Init()
    {
    }

    void ForwardRenderPass::Destroy()
    {
        mDstFB.Destroy();
    }

    void ForwardRenderPass::Render(const RenderingContext &rContext) const
    {
    }
}
