#include <Engine/GL/Texture2D.h>

#include <GL/glew.h>

using namespace std;
using namespace glm;

namespace engine
{
    Texture2D::Texture2D(const char *file, bool mipmaps)
        : Texture(GL_TEXTURE_2D)
    {
        InitFromFile(Target(), file, mipmaps);
    }

    Texture2D::Texture2D(void)
        : Texture(GL_TEXTURE_2D)
    {
    }

    void Texture2D::Init(const char *file, bool mipmaps)
    {
        InitFromFile(Target(), file, mipmaps);
    }

    void Texture2D::Init(uint internalFormat, ivec2 size, uint format, uint type)
    {
        InitEmpty(Target(), internalFormat, size, format, type);
    }

    void Texture2D::Resize(ivec2 size)
    {
        mSize = size;
        BindToSlot(30);
        glTexImage2D(Target(), 0, mInternalFormat, mSize.x, mSize.y, 0, mFormat, mType, nullptr);
        UnBindFromSlot(30);
    }
}
