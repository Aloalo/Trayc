#include <Engine/GL/Texture.h>

#include <IL/il.h>
#include <IL/ilu.h>
#include <GL/glew.h>

#include <string>
#include <iostream>
#include <vector>

using namespace std;
using namespace glm;

namespace engine
{
    Texture2D::Texture2D(const char *file)
    {
        init(FileImageLoader(file), file);
    }

    Texture2D::Texture2D(void)
        : mID(0)
    {
    }

    void Texture2D::init(const ImageLoader &imgl, const char *name)
    {
        unsigned int imgid;
        ilGenImages(1, &imgid);
        ilBindImage(imgid);
        try
        {
            if(!imgl.load())
                cerr << "Failed to load texture: " << name << endl;
            if(!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
                cerr << "Failed to convert texture: " << name << endl;
            glGenTextures(1, &mID);
            mSize = ivec2(ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
            mInternalFormat = GL_RGBA8;
            mFormat = GL_RGBA;
            mType = GL_UNSIGNED_BYTE;

            Bind();
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    mInternalFormat,
                    mSize.x,
                    mSize.y,
                    0,
                    mFormat,
                    mType,
                    ilGetData());
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            UnBind();
        }
        catch(const char *)
        {
            cerr << "Loading " << name << " failed: " << iluErrorString(ilGetError()) << endl;
        }
        ilDeleteImages(1, &imgid);
    }

    void Texture2D::Bind() const
    {
        glBindTexture(GL_TEXTURE_2D, mID);
    }

    void Texture2D::UnBind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture2D::Init(const char *name)
    {
        init(FileImageLoader(name), name);
    }

    void Texture2D::Init(uint internalFormat, ivec2 size, uint format, uint type)
    {
        mSize = size;
        mInternalFormat = internalFormat;
        mFormat = format;
        mType = type;

        glGenTextures(1, &mID);
        Bind();
        {
            glTexImage2D(GL_TEXTURE_2D,
                0,
                mInternalFormat,
                mSize.x,
                mSize.y,
                0,
                mFormat,
                mType,
                nullptr);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        UnBind();
    }

    void Texture2D::Resize(ivec2 size)
    {
        mSize = size;
        glBindTexture(GL_TEXTURE_2D, mID);
        glTexImage2D(GL_TEXTURE_2D, 0, mFormat, mSize.x, mSize.y, 0, mInternalFormat, mType, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture2D::Delete()
    {
        glDeleteTextures(1, &mID);
        mID = 0;
    }

    uint Texture2D::ID() const
    {
        return mID;
    }

    ivec2 Texture2D::Size() const
    {
        return mSize;
    }

    Texture2D::FileImageLoader::FileImageLoader(const char *file)
        : file(file)
    {
    }

    bool Texture2D::FileImageLoader::load() const
    {
        return ilLoadImage((const ILstring)file) != 0;
    }
}
