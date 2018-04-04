
#ifndef EN_GEOMETRYSHADER_H
#define EN_GEOMETRYSHADER_H

#include <Engine/GL/Shader.h>

namespace engine
{
    class GeometryShader :
        public Shader
    {
    public:
        GeometryShader(const char *name, const Defines &defines, const Constants &constants);
        ~GeometryShader(void);

        const char* GetExtension() const override;
        const char* GetTypeString() const override;
        GLenum GetGlType() const override;
    };
}

#endif
