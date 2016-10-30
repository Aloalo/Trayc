

#include <Engine/GL/FragmentShader.h>

namespace engine
{
    FragmentShader::FragmentShader(const char *name, const Defines &defines)
    {
        Init(name, defines);
    }

    FragmentShader::~FragmentShader(void)
    {
    }

    const char* FragmentShader::GetExtension() const
    {
        return ".frag";
    }

    const char* FragmentShader::GetTypeString() const
    {
        return "fragment";
    }

    GLenum FragmentShader::GetGlType() const
    {
        return GL_FRAGMENT_SHADER;
    }
}
