
#include <Engine/GL/TextureSampler.h>
#include <Engine/Utils/Setting.h>


namespace engine
{
    TextureSampler::TextureSampler(void)
        : mID(0)
    {
    }

    void TextureSampler::Init()
    {
        if(mID == 0) {
            glGenSamplers(1, &mID);
        }
    }

    void TextureSampler::Destroy()
    {
        glDeleteSamplers(1, &mID);
    }

    void TextureSampler::BindToSlot(GLuint unit) const
    {
        glBindSampler(unit, mID);
    }

    void TextureSampler::Parameterf(GLenum pname, GLfloat param) const
    {
        glSamplerParameterf(mID, pname, param);
    }

    void TextureSampler::Parameteri(GLenum pname, GLint param) const
    {
        glSamplerParameteri(mID, pname, param);
    }

    void TextureSampler::InitForDiffuse()
    {
        Init();

        glSamplerParameteri(mID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(mID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glSamplerParameteri(mID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glSamplerParameteri(mID, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glSamplerParameterf(mID, GL_TEXTURE_MAX_ANISOTROPY_EXT, Setting<float>("anisotropicFiltering"));
    }

    void TextureSampler::InitForDataTexture()
    {
        Init();

        glSamplerParameteri(mID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(mID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glSamplerParameteri(mID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(mID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
}
