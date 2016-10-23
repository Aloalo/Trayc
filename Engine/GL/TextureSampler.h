#ifndef EN_TEXTURE_SAMPLER_H
#define EN_TEXTURE_SAMPLER_H

#include <GL/glew.h>

namespace engine
{
    // Contains texture sampling params
    class TextureSampler
    {
    public:
        TextureSampler(void);

        void Init();
        void Destroy();

        void BindToSlot(GLuint unit) const;
        void Parameterf(GLenum pname, GLfloat param) const;
        void Parameteri(GLenum pname, GLint param) const;

        // Initalisation for diffuse maps
        // Linear mipmap mag filter, anisotropic min filter, wrap repeat
        void InitForDiffuse();

        // Initalisation for data textures such as g textures
        // Linear mag filter, Linear min filter, wrap clamp
        void InitForDataTexture();

        void InitForShadowmapTexture();

    private:
        GLuint mID;
    };
}

#endif

