/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/GL/FragmentShader.h>

namespace engine
{
    FragmentShader::FragmentShader(const char *name)
    {
        Init(name);
    }

    FragmentShader::FragmentShader(const std::string &source, const char *name)
    {
        Init2(source.c_str(), name);
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
