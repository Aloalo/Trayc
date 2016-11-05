
#include <Engine/GL/Shader.h>
#include <Engine/Utils/Utilities.h>

#include <easylogging++.h>

#include <string>
#include <fstream>
#include <sstream>

using namespace std;

namespace engine
{
    const std::string Shader::mHeader = "#version 330 core\nprecision highp float;\n";

    Shader::~Shader(void)
    {
        glDeleteShader(mID);
    }

    void Shader::Init(const char *name, const Defines &defines)
    {
        string sn(name);
        sn += GetExtension();
        StringReplace(sn, "\\", "/");

        ifstream in(sn, ifstream::in);
        if(in)
        {
            stringstream strStream;
            strStream << in.rdbuf();
            string source(strStream.str());
            source.erase(source.find_last_of('}')+1, source.length());
            InsertDefines(source, defines);

            const string path = sn.substr(0, sn.find_last_of("/") + 1);
            ExpandIncludes(path, source);
            AddHeader(source);

            Init2(source.c_str(), name);
        }
        else {
            LOG(ERROR) << "[Shader::Init] No shader file found: " << sn;
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

            LOG(ERROR) << "[Shader::Init2] Compile failure in " << GetTypeString() << " shader " << name << endl << strInfoLog;
            LOG(ERROR) << "[Shader::Init2] " << endl << src;
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

    static inline string FileToString(const string &fname)
    {
        ifstream in(fname, ifstream::in);
        if(in)
        {
            stringstream strStream;
            strStream << in.rdbuf();
            string source(strStream.str());
            auto pos = source.find_last_of('}');
            pos += (source[pos + 1] == ';');
            source.erase(pos + 1, source.length());
            return source;
        }
        LOG(ERROR) << "[FileToString] No include file found: " << fname;
        return string();
    }

    void Shader::ExpandIncludes(const string &path, string &source)
    {
        size_t pos = source.find("#include");

        while(pos != string::npos)
        {
            const size_t firstQuote = source.find("\"", pos);
            const size_t secondQuote = source.find("\"", firstQuote + 1);
            const size_t len = secondQuote - firstQuote - 1;

            const string includeFile = FileToString(path + source.substr(firstQuote + 1, len));
            StringReplace(source, source.substr(pos, secondQuote - pos + 1), includeFile);

            pos = source.find("#include");
        }
    }

    void Shader::AddHeader(std::string &source)
    {
        source = mHeader + source;
    }
}
