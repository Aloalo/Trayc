#include <Engine/GL/ScreenBuffer.h>

using namespace glm;

namespace engine
{
    ScreenBuffer::ScreenBuffer(void) :
        mVAO(GL_STATIC_DRAW)
    {
    }

    ScreenBuffer::~ScreenBuffer(void)
    {
        mProgram.Delete();
    }

    void ScreenBuffer::Init(int width, int height)
    {
        // Init FBO
        mFBO.Init(width, height);
        // Init Program
        mProgram.Init("screenBuffer", nullptr, "texture");
        mProgram.Use();
        mProgram.SetUniform("tex", 1);
        Program::Unbind();
        // Init VAO
        mVAO.AddAttribute(VertexAttribDef(0, 2, GL_FLOAT, GL_FALSE));
        mVAO.Init(4, 4);
        const vec2 quad[4] = { vec2(-1.f, -1.f), vec2(1.f, -1.f), vec2(-1.f, 1.f), vec2(1.f, 1.f) };
        mVAO.SetVertices(quad, 0, 4);
    }

    void ScreenBuffer::Resize(int width, int height)
    {
        // Resize Framebuffer
        mFBO.Resize(width, height);
        // Set frame buffer viewport
        mFBO.Bind();
        glViewport(0, 0, width, height);
        FrameBuffer::UnBind();
    }

    void ScreenBuffer::SetAsRenderTarget() const
    {
        mFBO.Bind();
        glViewport(0, 0, mFBO.Width(), mFBO.Height());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void ScreenBuffer::Render(int width, int height) const
    {
        FrameBuffer::UnBind();
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE1);
        mFBO.BindTexture();
        mProgram.Use();
        mVAO.Render(GL_TRIANGLE_STRIP);
        Program::Unbind();
    }
}
