/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_PROGRAM_H
#define EN_PROGRAM_H

#include <Engine/GL/VertexShader.h>
#include <Engine/GL/GeometryShader.h>
#include <Engine/GL/FragmentShader.h>
//#include "TextureSampler.h"
#include <glm/glm.hpp>
#include <map>

namespace engine
{
    class Program
    {
    public:
        GLuint id;

        Program(const VertexShader &vs, const FragmentShader &fs);
        Program(const VertexShader &vs, const GeometryShader &gs, const FragmentShader &fs);
        Program(const char *name);
        ~Program(void);

        void Use() const;

        GLint GetUniformLocation(const GLchar *name);
        GLuint GetUniformBlockLocation(const GLchar *name);
        GLint GetUniformi(const GLchar *name);

        void SetUniformBlockBinding(const GLchar *name, GLuint bindingPoint);

        void SetUniform(const GLchar *name, GLint x);
        void SetUniform(const GLchar *name, GLfloat x);
        void SetUniform(const GLchar *name, const glm::vec2 &x);
        void SetUniform(const GLchar *name, const glm::vec3 &x);
        void SetUniform(const GLchar *name, const glm::vec4 &x);
        void SetUniform(const GLchar *name, const glm::mat3 &x);
        void SetUniform(const GLchar *name, const glm::mat4 &x);

        void SetUniform(const GLchar *name, GLsizei cnt, const GLfloat *x);
        void SetUniform(const GLchar *name, GLsizei cnt, const glm::vec2 *x);
        void SetUniform(const GLchar *name, GLsizei cnt, const glm::vec3 *x);
        void SetUniform(const GLchar *name, GLsizei cnt, const glm::vec4 *x);
        //void bindSamplerObjectToSampler(const char *samplerName, const TextureSampler& tex);

    private:
        void Init(const VertexShader *vs, const GeometryShader *gs, const FragmentShader *fs, const char *name = NULL);

        void Attach(const Shader &sh) const;
        void Detach(const Shader &sh) const;

        std::map<const GLchar*, GLint> uniformLocations;
    };
}

#endif
