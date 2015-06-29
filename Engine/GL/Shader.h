/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_SHADER_H
#define EN_SHADER_H

#include <GL/glew.h>

namespace engine
{
    class Shader
    {
    public:
        virtual ~Shader(void);

        virtual const char* GetExtension() const = 0;
        virtual const char* GetTypeString() const = 0;
        virtual GLenum GetGlType() const = 0;

    protected:
        friend class Program;

        GLuint mID;
        void Init(const char *name);
        void Init2(const GLchar *src, const char *name, GLint len = 0);

    };
}

#endif
