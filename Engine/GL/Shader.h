
#ifndef EN_SHADER_H
#define EN_SHADER_H

#include <GL/glew.h>
#include <vector>

namespace engine
{
    class Shader
    {
    public:
        using Defines = std::vector<std::string>;
        using Constant = std::pair<std::string, size_t>;
        using Constants = std::vector<Constant>;

        virtual ~Shader(void);

        virtual const char* GetExtension() const = 0;
        virtual const char* GetTypeString() const = 0;
        virtual GLenum GetGlType() const = 0;

        const std::vector<std::string>& GetUniformBlockNames() const;

    protected:
        friend class Program;

        GLuint mID;
        void Init(const char *name, const Defines &defines, const Constants &constants);
        void Init2(const GLchar *src, const char *name, GLint len = 0);

    private:
        std::vector<std::string> mUniformBlocks;
        void FindUniformBlocks(const std::string &source);
        static void InsertDefines(std::string &source, const Defines &defines);
        static void InsertConstants(std::string &source, const Constants &constants);
        static void ExpandIncludes(const std::string &path, std::string &source);
        static const std::string mHeader;
        static void AddHeader(std::string &source);
    };
}

#endif
