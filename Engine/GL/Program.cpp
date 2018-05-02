
#include <Engine/GL/Program.h>
#include <Engine/Core/Defines.h>
#include <Engine/Utils/ErrorCheck.h>
#include <Engine/Utils/UniformBuffers.h>
#include <easylogging++.h>
#include <fstream>
#include <set>
#include <iterator>

using namespace glm;
using namespace std;

namespace engine
{
    GLuint Program::mBoundProgram = 0;

    Program::Program(void)
        : mID(0)
    {
    }

    void Program::Destroy()
    {
        glDeleteProgram(mID);
        mID = 0;
    }

    void Program::Init(const VertexShader &vs, const GeometryShader &gs, const FragmentShader &fs, const char *name)
    {
        Destroy();
        mID = glCreateProgram();
        mName = string(name);

        Attach(vs);
        Attach(gs);
        Attach(fs);

        LinkProgram(name);

        Detach(vs);
        Detach(gs);
        Detach(fs);

        CacheUniforms();
        set<string> ubNames(vs.GetUniformBlockNames().begin(), vs.GetUniformBlockNames().end());
        std::copy(gs.GetUniformBlockNames().begin(), gs.GetUniformBlockNames().end(), std::inserter(ubNames, ubNames.end()));
        std::copy(fs.GetUniformBlockNames().begin(), fs.GetUniformBlockNames().end(), std::inserter(ubNames, ubNames.end()));
        BindUniformBuffers(ubNames);
    }

    void Program::Init(const VertexShader &vs, const FragmentShader &fs, const char *name)
    {
        Destroy();
        mID = glCreateProgram();
        mName = string(name);

        Attach(vs);
        Attach(fs);

        LinkProgram(name);

        Detach(vs);
        Detach(fs);

        CacheUniforms();
        set<string> ubNames(vs.GetUniformBlockNames().begin(), vs.GetUniformBlockNames().end());
        std::copy(fs.GetUniformBlockNames().begin(), fs.GetUniformBlockNames().end(), std::inserter(ubNames, ubNames.end()));
        BindUniformBuffers(ubNames);
    }

    void Program::LinkProgram(const char *name)
    {
        glLinkProgram(mID);
        GLint status;
        glGetProgramiv(mID, GL_LINK_STATUS, &status);
        if(status == GL_FALSE)
        {
            GLint infoLogLength;
            glGetProgramiv(mID, GL_INFO_LOG_LENGTH, &infoLogLength);

            GLchar *strInfoLog = new GLchar[infoLogLength + 1];
            glGetProgramInfoLog(mID, infoLogLength, nullptr, strInfoLog);

            LOG(ERROR) << "[Program::LinkProgram] Linking failure in program " << name << ":" << endl << strInfoLog;
            delete[] strInfoLog;
        }
    }

    void Program::CacheUniforms()
    {
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
            mUniformLocations[string(uniformName)] = glGetUniformLocation(mID, uniformName);
        }
    }

    void Program::BindUniformBuffers(const std::set<std::string> &ubNames) const
    {
        for(const string &ubName : ubNames) {
            const UniformBuffer *buffer = UniformBuffers::Get().GetUniformBuffer(ubName);
            SetUniformBlockBinding(ubName.c_str(), buffer->GetBlockBinding());
        }
    }

    void Program::Init(const char *name, const Shader::Defines &defines, const Shader::Constants &constants)
    {
        mName = string(name);
        string sn(name);
        sn += ".geom";
        ifstream f(sn.c_str());

        if(f.good())
            Init(VertexShader(name, defines, constants), GeometryShader(name, defines, constants), FragmentShader(name, defines, constants), name);
        else
            Init(VertexShader(name, defines, constants), FragmentShader(name, defines, constants), name);

        f.close();
    }

    void Program::Init(const string &vsName, const string &fsName, const Shader::Defines &defines, const Shader::Constants &constants)
    {
        mName = "(" + vsName + ", " + fsName + ")";
        Init(VertexShader(vsName.c_str(), defines, constants), FragmentShader(fsName.c_str(), defines, constants));
    }

    void Program::Use() const
    {
        if(mID != mBoundProgram) {
            mBoundProgram = mID;
            glUseProgram(mID);
        }
    }

    void Program::Unbind()
    {
        glUseProgram(0);
        mBoundProgram = 0;
    }

    GLuint Program::GetUniformBlockLocation(const GLchar *name) const
    {
        return glGetUniformBlockIndex(mID, name);
    }

    void Program::SetUniformBlockBinding(const GLchar *name, GLuint bindingPoint) const
    {
        const GLuint blockIndex = GetUniformBlockLocation(name);
        if(blockIndex != GL_INVALID_INDEX) {
            glUniformBlockBinding(mID, blockIndex, bindingPoint);
        }
    #if PRODUCTION
        if(blockIndex == GL_INVALID_INDEX) {
            LOG(WARNING) << "[Program::GetUniformBlockLocation] Trying to bind unexisting uniform block " << name << " to program " << mName;
        }
    #endif
    }

    GLint Program::GetUniformi(const GLchar *name) const
    {
        GLint ret;
        glGetUniformiv(mID, mUniformLocations.find(name)->second, &ret);
        return ret;
    }

    void Program::SetUniform(const string &name, GLint x) const
    {
        glProgramUniform1i(mID, mUniformLocations.find(name)->second, x);
    }

    void Program::SetUniform(const string &name, GLfloat x) const
    {
        glProgramUniform1f(mID, mUniformLocations.find(name)->second, x);
    }

    void Program::SetUniform(const string &name, const vec2 &x) const
    {
        glProgramUniform2f(mID, mUniformLocations.find(name)->second, x.x, x.y);
    }

    void Program::SetUniform(const string &name, const vec3 &x) const
    {
        glProgramUniform3f(mID, mUniformLocations.find(name)->second, x.x, x.y, x.z);
    }

    void Program::SetUniform(const string &name, const vec4 &x) const
    {
        glProgramUniform4f(mID, mUniformLocations.find(name)->second, x.x, x.y, x.z, x.w);
    }

    void Program::SetUniform(const string &name, const mat3 &x) const
    {
        glProgramUniformMatrix3fv(mID, mUniformLocations.find(name)->second, 1, GL_FALSE, (float*)&x);
    }

    void Program::SetUniform(const string &name, const mat4 &x) const
    {
        glProgramUniformMatrix4fv(mID, mUniformLocations.find(name)->second, 1, GL_FALSE, (float*)&x);
    }

    void Program::SetUniform(const string &name, GLsizei cnt, const GLfloat *x) const
    {
        glProgramUniform1fv(mID, mUniformLocations.find(name)->second, cnt, x);
    }

    void Program::SetUniform(const string &name, GLsizei cnt, const vec2 *x) const
    {
        glProgramUniform2fv(mID, mUniformLocations.find(name)->second, cnt, (float*)x);
    }

    void Program::SetUniform(const string &name, GLsizei cnt, const vec3 *x) const
    {
        glProgramUniform3fv(mID, mUniformLocations.find(name)->second, cnt, (float*)x);
    }

    void Program::SetUniform(const string &name, GLsizei cnt, const vec4 *x) const
    {
        glProgramUniform4fv(mID, mUniformLocations.find(name)->second, cnt, (float*)x);
    }

    GLuint Program::GetID() const
    {
        return mID;
    }

    void Program::Attach(const Shader &sh) const
    {
        glAttachShader(mID, sh.mID);
    }

    void Program::Detach(const Shader &sh) const
    {
        glDetachShader(mID, sh.mID);
    }
}
