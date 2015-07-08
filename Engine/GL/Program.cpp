/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/GL/Program.h>
#include <Engine/Utils/ErrorCheck.h>
#include <iostream>
#include <fstream>

using namespace glm;
using namespace std;

namespace engine
{
    Program::Program(void)
        : mID(0)
    {
    }

    Program::Program(const VertexShader &vs, const GeometryShader &gs, const FragmentShader &fs)
        : mID(0)
    {
        Init(&vs, &gs, &fs);
    }

    Program::Program(const VertexShader &vs, const FragmentShader &fs)
        : mID(0)
    {
        Init(&vs, nullptr, &fs);
    }

    Program::Program(const char *name)
        : mID(0)
    {
        Init(name);
    }

    void Program::Delete()
    {
        glDeleteProgram(mID);
        mID = 0;
    }

    void Program::Init(const VertexShader *vs, const GeometryShader *gs, const FragmentShader *fs, const char *name)
    {
        Delete();

        mID = glCreateProgram();
        Attach(*vs);
        if(gs)
            Attach(*gs);
        Attach(*fs);
        glLinkProgram(mID);
        GLint status;
        glGetProgramiv(mID, GL_LINK_STATUS, &status);
        if(status == GL_FALSE)
        {
            GLint infoLogLength;
            glGetProgramiv(mID, GL_INFO_LOG_LENGTH, &infoLogLength);

            GLchar *strInfoLog = new GLchar[infoLogLength+1];
            glGetProgramInfoLog(mID, infoLogLength, nullptr, strInfoLog);

            cerr << "Linking failure in program " << name << ":" << endl << strInfoLog << endl;
            delete[] strInfoLog;
        }
        Detach(*vs);
        if(gs)
            Detach(*gs);
        Detach(*fs);

        //Cache all the uniforms
        GLint uniformCount;
        glGetProgramiv(mID, GL_ACTIVE_UNIFORMS, &uniformCount);
        const GLsizei bufferSize = 100;
        for(GLint i = 0; i < uniformCount; ++i)
        {
            GLchar uniformName[bufferSize];
            GLsizei length;
            GLint size;
            GLenum type;
            glGetActiveUniform(mID, i, bufferSize, &length, &size, &type, uniformName);
            mUniformLocations[string(uniformName)] = i;
        }
    }

    void Program::Init(const char *name)
    {
        string sn(name);
        sn += ".geom";
        ifstream f(sn.c_str());

        if(f.good())
            Init(&VertexShader(name), &GeometryShader(name), &FragmentShader(name), name);
        else
            Init(&VertexShader(name), nullptr, &FragmentShader(name), name);

        f.close();
    }

    void Program::Use() const
    {
        glUseProgram(mID);
    }

    void Program::Unbind()
    {
        glUseProgram(0);
    }

    /*GLint Program::GetUniformLocation(const GLchar *name)
    {
    const auto it = mUniformLocations.find(name);
    if(it != mUniformLocations.end())
    return it->second;
    else
    {
    const GLint location = glGetUniformLocation(mID, name);
    if(location == -1)
    {
    cerr << "Uniform '" << name << "' not in shader '" << mID << "'" << endl;
    }
    mUniformLocations[name] = location;
    return location;
    }
    }*/

    GLuint Program::GetUniformBlockLocation(const GLchar *name)
    {
        return glGetUniformBlockIndex(mID, name);
    }

    GLint Program::GetUniformi(const GLchar *name)
    {
        GLint ret;
        glGetUniformiv(mID, mUniformLocations[name], &ret);
        return ret;
    }

    void Program::SetUniformBlockBinding(const GLchar *name, GLuint bindingPoint)
    {
        glUniformBlockBinding(mID, GetUniformBlockLocation(name), bindingPoint);
    }

    void Program::SetUniform(const string &name, GLint x) const
    {
        glUniform1i(mUniformLocations.find(name)->second, x);
    }

    void Program::SetUniform(const string &name, GLfloat x) const
    {
        glUniform1f(mUniformLocations.find(name)->second, x);
    }

    void Program::SetUniform(const string &name, const vec2 &x) const
    {
        glUniform2f(mUniformLocations.find(name)->second, x.x, x.y);
    }

    void Program::SetUniform(const string &name, const vec3 &x) const
    {
        glUniform3f(mUniformLocations.find(name)->second, x.x, x.y, x.z);
    }

    void Program::SetUniform(const string &name, const vec4 &x) const
    {
        glUniform4f(mUniformLocations.find(name)->second, x.x, x.y, x.z, x.w);
    }

    void Program::SetUniform(const string &name, const mat3 &x) const
    {
        glUniformMatrix3fv(mUniformLocations.find(name)->second, 1, GL_FALSE, (float*)&x);
    }

    void Program::SetUniform(const string &name, const mat4 &x) const
    {
        glUniformMatrix4fv(mUniformLocations.find(name)->second, 1, GL_FALSE, (float*)&x);
    }

    void Program::SetUniform(const string &name, GLsizei cnt, const GLfloat *x) const
    {
        glUniform1fv(mUniformLocations.find(name)->second, cnt, x);
    }

    void Program::SetUniform(const string &name, GLsizei cnt, const vec2 *x) const
    {
        glUniform2fv(mUniformLocations.find(name)->second, cnt, (float*)x);
    }

    void Program::SetUniform(const string &name, GLsizei cnt, const vec3 *x) const
    {
        glUniform3fv(mUniformLocations.find(name)->second, cnt, (float*)x);
    }

    void Program::SetUniform(const string &name, GLsizei cnt, const vec4 *x) const
    {
        glUniform4fv(mUniformLocations.find(name)->second, cnt, (float*)x);
    }

   /* void Program::bindSamplerObjectToSampler(const char *name, const TextureSampler &tex)
    {
        int texUnit = GetUniformi(name);
        glActiveTexture(GL_TEXTURE0 + texUnit);
        glBindSampler(texUnit, tex.GetID());
    }*/

    void Program::Attach(const Shader &sh) const
    {
        glAttachShader(mID, sh.mID);
    }

    void Program::Detach(const Shader &sh) const
    {
        glDetachShader(mID, sh.mID);
    }
}
