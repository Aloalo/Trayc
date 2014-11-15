/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_VERTEXSHADER_H
#define EN_VERTEXSHADER_H

#include <Engine/GL/Shader.h>

namespace engine
{
    class VertexShader :
        public Shader
    {
    public:
        VertexShader(const char *name);
        ~VertexShader(void);

        const char* GetExtension() const;
        const char* GetTypeString() const;
        GLenum GetGlType() const;
    };
}

#endif
