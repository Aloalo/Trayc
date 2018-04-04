
#include <Engine/GL/GeometryShader.h>

namespace engine
{
    GeometryShader::GeometryShader(const char *name, const Defines &defines, const Constants &constants)
    {
        Init(name, defines, constants);
    }

    GeometryShader::~GeometryShader(void)
    {
    }

    const char* GeometryShader::GetExtension() const
    {
        return ".geom";
    }

    const char* GeometryShader::GetTypeString() const
    {
        return "geometry";
    }

    GLenum GeometryShader::GetGlType() const
    {
        return GL_GEOMETRY_SHADER;
    }
}
