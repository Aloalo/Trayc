
#ifndef EN_PROGRAM_H
#define EN_PROGRAM_H

#include <Engine/GL/VertexShader.h>
#include <Engine/GL/GeometryShader.h>
#include <Engine/GL/FragmentShader.h>
#include <glm/glm.hpp>
#include <map>
#include <set>
#include <vector>

namespace engine
{
    class Program
    {
    public:
        Program(void);

        void Init(const VertexShader &vs, const GeometryShader &gs, const FragmentShader &fs, const char *name = "PROGRAM NOT NAMED");
        void Init(const VertexShader &vs, const FragmentShader &fs, const char *name = "PROGRAM NOT NAMED");
        void Init(const char *name, const Shader::Defines &defines = Shader::Defines()); //init from filename
        void Init(const std::string &vsName, const std::string &fsName, const Shader::Defines &defines = Shader::Defines()); //init from filename

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
        void BindUniformBuffers(const std::set<std::string> &ubNames) const;
        void LinkProgram(const char *name);

        GLuint mID;
        std::string mName;
        std::map<std::string, GLint> mUniformLocations;

        static GLuint mBoundProgram;
    };
}

#endif
