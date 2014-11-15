/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/GL/Shader.h>
#include <string>
#include <iostream>
#include <fstream>

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
            unsigned long long int size = 0ull;
            in.seekg(0, ios::end);
            ifstream::pos_type szp = in.tellg();
            in.seekg(0, ios::beg);
            size = static_cast<unsigned long long int>(szp);

            char *source = new char[static_cast<size_t>(size)];
            memset(source, 0, static_cast<size_t>(size));
            in.read(source, static_cast<streamsize>(size));

            Init2(source, name);
            delete[] source;
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
