
#ifndef EN_VERTEXSHADER_H
#define EN_VERTEXSHADER_H

#include <Engine/GL/Shader.h>

namespace engine
{
    class VertexShader :
        public Shader
    {
    public:
        VertexShader(const char *name, const Defines &defines, const Constants &constants);
        ~VertexShader(void);

        const char* GetExtension() const override;
        const char* GetTypeString() const override;
        GLenum GetGlType() const override;
    };
}

#endif
