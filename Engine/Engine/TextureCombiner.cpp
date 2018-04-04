
#include <Engine/Engine/TextureCombiner.h>

using namespace glm;
using namespace std;

namespace engine
{
    VertexArray TextureCombiner::mVAO = VertexArray(GL_STATIC_DRAW);

    TextureCombiner::TextureCombiner(void)
    {
    }

    TextureCombiner::~TextureCombiner(void)
    {
    }

    void TextureCombiner::Init(const char *progName, const Shader::Defines &defines, const Shader::Constants &constants)
    {
        mProgram.Init(progName, defines, constants);
        Init();
    }

    void TextureCombiner::Init(const char *vsName, const char *fsName, const Shader::Defines &defines, const Shader::Constants &constants)
    {
        mProgram.Init(VertexShader(vsName, defines, constants), FragmentShader(fsName, defines, constants));
        Init();
    }

    void TextureCombiner::Init(const string &vsName, const string &fsName, const Shader::Defines &defines, const Shader::Constants &constants)
    {
        mProgram.Init(VertexShader(vsName.c_str(), defines, constants), FragmentShader(fsName.c_str(), defines, constants));
        Init();
    }

    void TextureCombiner::Destroy()
    {
        mProgram.Destroy();
    }

    const Program& TextureCombiner::Prog() const
    {
        return mProgram;
    }

    void TextureCombiner::SetTexture(int idx, const Texture *tex)
    {
        tex->BindToSlot(idx);
    }

    void TextureCombiner::Draw() const
    {
        mProgram.Use();
        mVAO.Render(GL_TRIANGLE_FAN);
    }

    void TextureCombiner::DestroyVAO()
    {
        mVAO.Destroy();
    }

    void TextureCombiner::Init()
    {
        if(mVAO.Capacity() == 0)
        {
            static const vec2 quad[4] =
            {
                vec2(1.0f, 1.0f),
                vec2(-1.0f, 1.0f),
                vec2(-1.0f,-1.0f),
                vec2(1.0f,-1.0f)
            };
            mVAO.AddAttribute(VertexAttribDef(0, 2, GL_FLOAT, GL_FALSE));
            mVAO.Init(4, 4);
            mVAO.SetVertices(quad, 0, 4);
        }
    }
}
