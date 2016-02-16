/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_PROGRAM_H
#define EN_PROGRAM_H

#include <Engine/GL/VertexShader.h>
#include <Engine/GL/GeometryShader.h>
#include <Engine/GL/FragmentShader.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>

namespace engine
{
    class Program
    {
    public:
        Program(void);

        void Init(const VertexShader &vs, const GeometryShader &gs, const FragmentShader &fs, const char *name = nullptr);
        void Init(const VertexShader &vs, const FragmentShader &fs, const char *name = nullptr);
        void Init(const char *name, const Shader::Defines &defines = Shader::Defines()); //init from filename

        void Destroy();

        void Use() const;
        static void Unbind();

        GLuint GetUniformBlockLocation(const GLchar *name) const;
        void SetUniformBlockBinding(const GLchar *name, GLuint bindingPoint) const;

        GLint GetUniformi(const GLchar *name) const;

        //bind the program before using these
        void SetUniform(const std::string &name, GLint x) const;
        void SetUniform(const std::string &name, GLfloat x) const;
        void SetUniform(const std::string &name, const glm::vec2 &x) const;
        void SetUniform(const std::string &name, const glm::vec3 &x) const;
        void SetUniform(const std::string &name, const glm::vec4 &x) const;
        void SetUniform(const std::string &name, const glm::mat3 &x) const;
        void SetUniform(const std::string &name, const glm::mat4 &x) const;

        void SetUniform(const std::string &name, GLsizei cnt, const GLfloat *x) const;
        void SetUniform(const std::string &name, GLsizei cnt, const glm::vec2 *x) const;
        void SetUniform(const std::string &name, GLsizei cnt, const glm::vec3 *x) const;
        void SetUniform(const std::string &name, GLsizei cnt, const glm::vec4 *x) const;

        GLuint GetID() const;

    private:
        void Attach(const Shader &sh) const;
        void Detach(const Shader &sh) const;

        void CacheUniforms();
        void LinkProgram(const char *name);

        GLuint mID;
        std::map<std::string, GLint> mUniformLocations;

        static GLuint mBoundProgram;
    };
}

#endif
