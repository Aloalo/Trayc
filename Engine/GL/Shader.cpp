/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/GL/Shader.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

namespace engine
{
    Shader::~Shader(void)
    {
        glDeleteShader(mID);
    }

    void Shader::Init(const char *name, const Defines &defines)
    {
        string sn(name);
        sn += GetExtension();

        ifstream in(sn, ifstream::in);
        if(in)
        {
            stringstream strStream;
            strStream << in.rdbuf();
            string source(strStream.str());
            source.erase(source.find_last_of('}')+1, source.length());
            InsertDefines(source, defines);

            Init2(source.c_str(), name);
        }
        else
        {
            cerr << "No shader file found: " << sn << endl;
        }
        in.close();
    }

    void Shader::Init2(const GLchar *src, const char *name, GLint len)
    {
        mID = glCreateShader(GetGlType());
        if(len == 0)
            glShaderSource(mID, 1, &src, 0);
        else
            glShaderSource(mID, 1, &src, &len);
        glCompileShader(mID);

        GLint status;
        glGetShaderiv(mID, GL_COMPILE_STATUS, &status);
        if(status == GL_FALSE)
        {
            GLint infoLogLength;
            glGetShaderiv(mID, GL_INFO_LOG_LENGTH, &infoLogLength);

            GLchar *strInfoLog = new GLchar[infoLogLength+1];
            glGetShaderInfoLog(mID, infoLogLength, nullptr, strInfoLog);

            cerr << "Compile failure in " << GetTypeString() << " shader " << name << endl << strInfoLog << endl;
            delete[] strInfoLog;
        }
    }

    void Shader::InsertDefines(string &source, const Defines& defines)
    {
        string strDefines;

        for(const string &def : defines) {
            strDefines += "#define " + def + "\n";
        }

        source.insert(source.find("\n") + 1, strDefines);
    }
}
