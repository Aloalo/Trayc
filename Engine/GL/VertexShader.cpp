
#include <Engine/GL/VertexShader.h>

namespace engine
{
    VertexShader::VertexShader(const char *name, const Defines &defines)
    {
        Init(name, defines);
    }

    VertexShader::~VertexShader(void)
    {
    }

    const char* VertexShader::GetExtension() const
    {
        return ".vert";
    }

    const char* VertexShader::GetTypeString() const
    {
        return "vertex";
    }

    GLenum VertexShader::GetGlType() const
    {
        return GL_VERTEX_SHADER;
    }
}
