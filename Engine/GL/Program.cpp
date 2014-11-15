/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/GL/Program.h>
#include <Engine/Common/ErrorCheck.h>
#include <iostream>
#include <fstream>

using namespace glm;
using namespace std;

namespace engine
{
    Program::Program(const VertexShader &vs, const GeometryShader &gs, const FragmentShader &fs)
    {
        Init(&vs, &gs, &fs);
    }

    Program::Program(const VertexShader &vs, const FragmentShader &fs)
    {
        Init(&vs, NULL, &fs);
    }

    Program::Program(const char *name)
    {
        string sn(name);
        sn += ".geom";
        ifstream f(sn.c_str());

        if(f.good())
            Init(&VertexShader(name), &GeometryShader(name), &FragmentShader(name), name);
        else
            Init(&VertexShader(name), 0, &FragmentShader(name), name);

        f.close();
    }

    Program::~Program(void)
    {
    }

    void Program::Init(const VertexShader *vs, const GeometryShader *gs, const FragmentShader *fs, const char *name)
    {
        id = glCreateProgram();
        Attach(*vs);
        if(gs)
            Attach(*gs);
        Attach(*fs);
        glLinkProgram(id);
        GLint status;
        glGetProgramiv(id, GL_LINK_STATUS, &status);
        if(status == GL_FALSE)
        {
            GLint infoLogLength;
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);

            GLchar *strInfoLog = new GLchar[infoLogLength+1];
            glGetProgramInfoLog(id, infoLogLength, NULL, strInfoLog);

            cerr << "Linking failure in program " << name << ":" << endl << strInfoLog << endl;
            delete[] strInfoLog;
        }
        Detach(*vs);
        if(gs)
            Detach(*gs);
        Detach(*fs);
    }

    void Program::Use() const
    {
        glUseProgram(id);
    }

    GLint Program::GetUniformLocation(const GLchar *name)
    {
        const auto it = uniformLocations.find(name);
        if(it != uniformLocations.end())
            return it->second;
        else
        {
            const GLint location = glGetUniformLocation(id, name);
            uniformLocations[name] = location;
            return location;
        }
    }

    GLuint Program::GetUniformBlockLocation(const GLchar *name)
    {
        return glGetUniformBlockIndex(id, name);
    }

    GLint Program::GetUniformi(const GLchar *name)
    {
        GLint ret;
        glGetUniformiv(id, GetUniformLocation(name), &ret);
        return ret;
    }

    void Program::SetUniformBlockBinding(const GLchar *name, GLuint bindingPoint)
    {
        glUniformBlockBinding(id, GetUniformBlockLocation(name), bindingPoint);
    }

    void Program::SetUniform(const GLchar *name, GLint x)
    {
        glProgramUniform1i(id, GetUniformLocation(name), x);
    }

    void Program::SetUniform(const GLchar *name, GLfloat x)
    {
        glProgramUniform1f(id, GetUniformLocation(name), x);
    }

    void Program::SetUniform(const GLchar *name, const vec2 &x)
    {
        glProgramUniform2f(id, GetUniformLocation(name), x.x, x.y);
    }

    void Program::SetUniform(const GLchar *name, const vec3 &x)
    {
        glProgramUniform3f(id, GetUniformLocation(name), x.x, x.y, x.z);
    }

    void Program::SetUniform(const GLchar *name, const vec4 &x)
    {
        glProgramUniform4f(id, GetUniformLocation(name), x.x, x.y, x.z, x.w);
    }

    void Program::SetUniform(const GLchar *name, const mat3 &x)
    {
        glProgramUniformMatrix3fv(id, GetUniformLocation(name), 1, GL_FALSE, (float*)&x);
    }

    void Program::SetUniform(const GLchar *name, const mat4 &x)
    {
        glProgramUniformMatrix4fv(id, GetUniformLocation(name), 1, GL_FALSE, (float*)&x);
    }

    void Program::SetUniform(const GLchar *name, GLsizei cnt, const GLfloat *x)
    {
        glProgramUniform1fv(id, GetUniformLocation(name), cnt, x);
    }

    void Program::SetUniform(const GLchar *name, GLsizei cnt, const vec2 *x)
    {
        glProgramUniform2fv(id, GetUniformLocation(name), cnt, (float*)x);
    }

    void Program::SetUniform(const GLchar *name, GLsizei cnt, const vec3 *x)
    {
        glProgramUniform3fv(id, GetUniformLocation(name), cnt, (float*)x);
    }

    void Program::SetUniform(const GLchar *name, GLsizei cnt, const vec4 *x)
    {
        glProgramUniform4fv(id, GetUniformLocation(name), cnt, (float*)x);
    }

   /* void Program::bindSamplerObjectToSampler(const char *name, const TextureSampler &tex)
    {
        int texUnit = GetUniformi(name);
        glActiveTexture(GL_TEXTURE0 + texUnit);
        glBindSampler(texUnit, tex.GetID());
    }*/

    void Program::Attach(const Shader &sh) const
    {
        glAttachShader(id, sh.id);
    }

    void Program::Detach(const Shader &sh) const
    {
        glDetachShader(id, sh.id);
    }
}
