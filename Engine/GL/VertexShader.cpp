/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/GL/VertexShader.h>

namespace engine
{
    VertexShader::VertexShader(const char *name)
    {
        Init(name);
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
