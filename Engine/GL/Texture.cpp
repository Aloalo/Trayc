#include <Engine/GL/Texture.h>
#include <Engine/Utils/Setting.h>

#include <IL/il.h>
#include <IL/ilu.h>
#include <GL/glew.h>

#include <easylogging++.h>

#include <string>
#include <vector>
#include <algorithm>

using namespace std;
using namespace glm;

namespace engine
{
    unsigned int Texture2D::mBoundTextures[TextureType::CT_TEX_SLOTS] = {0};

    Texture2D::Texture2D(const char *file, bool mipmaps)
    {
        init(FileImageLoader(file), file, mipmaps);
    }

    Texture2D::Texture2D(void)
        : mID(0)
    {
    }

    void Texture2D::init(const ImageLoader &imgl, const char *file, bool mipmaps)
    {
        unsigned int imgid;
        ilGenImages(1, &imgid);
        ilBindImage(imgid);
        if(!imgl.load()) {
            LOG(ERROR) << "Failed to load texture: " << file;
            ilDeleteImages(1, &imgid);
            return;
        }
        if(!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE)) {
            LOG(ERROR) << "Failed to convert texture: " << file;
            ilDeleteImages(1, &imgid);
            return;
        }

        ILinfo ImageInfo;
        iluGetImageInfo(&ImageInfo);
        if(ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT) {
            iluFlipImage();
        }

        LOG(INFO) << "Loaded Texture: " << file;

        glGenTextures(1, &mID);
        mSize = ivec2(ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
        mInternalFormat = GL_RGBA;
        mFormat = GL_RGBA;
        mType = GL_UNSIGNED_BYTE;

        BindToSlot(0);
        {
            glTexImage2D(GL_TEXTURE_2D, 0, mInternalFormat, mSize.x, mSize.y, 0, mFormat, mType, ilGetData());
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            const int maxMipLevel = int(std::log2(max(mSize.x, mSize.y))) - Setting<int>("maxMipmapLevelMod");
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipmaps ? maxMipLevel : 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        }
        UnBindFromSlot(0);

        ilDeleteImages(1, &imgid);
    }

    void Texture2D::BindToSlot(int texSlot) const
    {
        if(mBoundTextures[texSlot] != mID) {
            glActiveTexture(GL_TEXTURE0 + texSlot);
            glBindTexture(GL_TEXTURE_2D, mID);
            mBoundTextures[texSlot] = mID;
        }
    }

    void Texture2D::UnBindFromSlot(int texSlot)
    {
        glActiveTexture(GL_TEXTURE0 + texSlot);
        glBindTexture(GL_TEXTURE_2D, 0);
        mBoundTextures[texSlot] = 0;
    }

    void Texture2D::Init(const char *file, bool mipmaps)
    {
        init(FileImageLoader(file), file, mipmaps);
    }

    void Texture2D::Init(uint internalFormat, ivec2 size, uint format, uint type)
    {
        mSize = size;
        mInternalFormat = internalFormat;
        mFormat = format;
        mType = type;

        glGenTextures(1, &mID);
        BindToSlot(0);
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
        UnBindFromSlot(0);
    }

    void Texture2D::Resize(ivec2 size)
    {
        mSize = size;
        BindToSlot(30);
        glTexImage2D(GL_TEXTURE_2D, 0, mInternalFormat, mSize.x, mSize.y, 0, mFormat, mType, nullptr);
        UnBindFromSlot(30);
    }

    void Texture2D::Destroy()
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
