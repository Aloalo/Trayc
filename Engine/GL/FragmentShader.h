/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_FRAGMENTSHADER_H
#define EN_FRAGMENTSHADER_H

#include <Engine/GL/Shader.h>

namespace engine
{
    class FragmentShader :
        public Shader
    {
    public:
        FragmentShader(const char *name);
        ~FragmentShader(void);

        const char* GetExtension() const;
        const char* GetTypeString() const;
        GLenum GetGlType() const;
    };
}

#endif
