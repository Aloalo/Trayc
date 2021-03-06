
#include <Engine/GL/Shader.h>
#include <Engine/Utils/Utilities.h>

#include <easylogging++.h>

#include <string>
#include <fstream>
#include <sstream>

using namespace std;

namespace engine
{
    const std::string Shader::mHeader = "#version 400 core\nprecision highp float;\n";

    Shader::~Shader(void)
    {
        glDeleteShader(mID);
    }

    const std::vector<std::string>& Shader::GetUniformBlockNames() const
    {
        return  mUniformBlocks;
    }

    void Shader::Init(const char *name, const Defines &defines, const Constants &constants)
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
            InsertConstants(source, constants);

            const string path = sn.substr(0, sn.find_last_of("/") + 1);
            ExpandIncludes(path, source);
            FindUniformBlocks(source);
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

        GLint infoLogLength = 0;
        glGetShaderiv(mID, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = nullptr;
        if (infoLogLength > 0) {
            strInfoLog = new GLchar[infoLogLength + 1];
            glGetShaderInfoLog(mID, infoLogLength, nullptr, strInfoLog);
        }

        GLint status;
        glGetShaderiv(mID, GL_COMPILE_STATUS, &status);
        if(status == GL_FALSE) {
            LOG(ERROR) << "[Shader::Init2] Compile failure in " << GetTypeString() << " shader " << name << endl << strInfoLog;
            delete[] strInfoLog;
        }
        else if (infoLogLength > 0) {
            LOG(WARNING) << "[Shader::Init2] Compile warning in " << GetTypeString() << " shader " << name << endl << strInfoLog;
            delete[] strInfoLog;
        }
    }

    void Shader::FindUniformBlocks(const string &source)
    {
        static const string ubTag = "layout(std140) uniform ";
        static const size_t ubTagLen = ubTag.size();

        size_t pos = source.find(ubTag);
        while(pos != string::npos)
        {
            pos += ubTagLen;
            const size_t nameEnd = source.find("\n", pos);
            const size_t len = nameEnd - pos;
            mUniformBlocks.push_back(source.substr(pos, len));
            pos = source.find("layout(std140) uniform ", pos + 1);
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

    void Shader::InsertConstants(string &source, const Constants &constants)
    {
        string strConstants;

        for (const Constant &c : constants) {
            strConstants += "#define " + c.first + " " + to_string(c.second) + "\n";
        }

        source.insert(source.find("\n") + 1, strConstants);
    }

    static inline string FileToString(const string &fname)
    {
        ifstream in(fname, ifstream::in);
        if(in)
        {
            stringstream strStream;
            strStream << in.rdbuf();
            string source(strStream.str());
            auto pos = max(source.find_last_of('}'), source.find_last_of("#endif"));
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
