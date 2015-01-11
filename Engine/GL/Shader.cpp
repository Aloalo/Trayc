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
        glDeleteShader(id);
    }

    void Shader::Init(const char *name)
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
        id = glCreateShader(GetGlType());
        if(len == 0)
            glShaderSource(id, 1, &src, 0);
        else
            glShaderSource(id, 1, &src, &len);
        glCompileShader(id);

        GLint status;
        glGetShaderiv(id, GL_COMPILE_STATUS, &status);
        if(status == GL_FALSE)
        {
            GLint infoLogLength;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);

            GLchar *strInfoLog = new GLchar[infoLogLength+1];
            glGetShaderInfoLog(id, infoLogLength, NULL, strInfoLog);

            cerr << "Compile failure in " << GetTypeString() << " shader " << name << endl << strInfoLog << endl;
            delete[] strInfoLog;
        }
    }
}
