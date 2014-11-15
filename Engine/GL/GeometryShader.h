/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_GEOMETRYSHADER_H
#define EN_GEOMETRYSHADER_H

#include <Engine/GL/Shader.h>

namespace engine
{
    class GeometryShader :
        public Shader
    {
    public:
        GeometryShader(const char *name);
        ~GeometryShader(void);

        const char* GetExtension() const;
        const char* GetTypeString() const;
        GLenum GetGlType() const;
    };
}

#endif
