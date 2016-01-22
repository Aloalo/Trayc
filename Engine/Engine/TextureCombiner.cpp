
#include <Engine/Engine/TextureCombiner.h>

using namespace glm;

namespace engine
{
    TextureCombiner::TextureCombiner(void)
        : mVAO(GL_STATIC_DRAW)
    {
    }

    TextureCombiner::~TextureCombiner(void)
    {
    }

    void TextureCombiner::Init(const char *progName)
    {
        mProgram.Init(progName);

        static const vec2 quad[4] =
        {
            vec2(-1.0f, 1.0f),
            vec2(1.0f, 1.0f),
            vec2(-1.0f, -1.0f),
            vec2(1.0f, -1.0f)
        };
        mVAO.AddAttribute(VertexAttribDef(0, 2, GL_FLOAT, GL_FALSE));
        mVAO.Init(4, 4);
        mVAO.SetVertices(quad, 0, 4);
    }

    void TextureCombiner::Destroy()
    {
        mProgram.Delete();
    }

    const Program&TextureCombiner::Prog() const
    {
        return mProgram;
    }

    void TextureCombiner::SetTexture(int idx, const Texture2D &tex) const
    {
        glActiveTexture(GL_TEXTURE0 + idx);
        tex.Bind();
    }

    void TextureCombiner::Draw(const FrameBuffer &FBO) const
    {
        FBO.Bind();
        mProgram.Use();
        mVAO.Render(GL_TRIANGLE_STRIP);
        Program::Unbind();
        FrameBuffer::UnBind();
    }

    void TextureCombiner::Draw() const
    {
        mProgram.Use();
        mVAO.Render(GL_TRIANGLE_STRIP);
        Program::Unbind();
    }

}
