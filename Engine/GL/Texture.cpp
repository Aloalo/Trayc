#include <Engine/GL/Texture2D.h>
#include <Engine/Utils/Setting.h>

#include <GL/glew.h>
#include <IL/il.h>
#include <IL/ilu.h>

#include <easylogging++.h>

#include <cmath>


using namespace glm;

namespace engine
{
    TextureDescription::TextureDescription(const ivec2 &size, uint internalFormat, uint format, uint type)
        : size(size), internalFormat(internalFormat), format(format), type(type)
    {
    }

    const int TextureDescription::operator<(const TextureDescription &other)
    {
        return memcmp(static_cast<const void*>(this), static_cast<const void*>(&other), sizeof(TextureDescription));
    }

    unsigned int Texture::mBoundTextures[TextureType::CT_TEX_SLOTS] = {0};

    Texture::Texture(uint target)
        : mSize(0), mInternalFormat(0), mFormat(0), mType(0), mID(0), mTarget(target)
    {
    }

    void Texture::Init()
    {
        glGenTextures(1, &mID);
    }

    void Texture::BindToSlot(int texSlot) const
    {
        if(mBoundTextures[texSlot] != mID) {
            glActiveTexture(GL_TEXTURE0 + texSlot);
            glBindTexture(GL_TEXTURE_2D, mID);
            mBoundTextures[texSlot] = mID;
        }
    }

    void Texture::UnBindFromSlot(int texSlot)
    {
        glActiveTexture(GL_TEXTURE0 + texSlot);
        glBindTexture(GL_TEXTURE_2D, 0);
        mBoundTextures[texSlot] = 0;
    }

    void Texture::Destroy()
    {
        glDeleteTextures(1, &mID);
        mID = 0;
    }

    ivec2 Texture::Size() const
    {
        return mSize;
    }

    uint Texture::ID() const
    {
        return mID;
    }

    uint Texture::Target() const
    {
        return mTarget;
    }

    TextureDescription Texture::GetTextureDescription() const
    {
        return TextureDescription(mSize, mInternalFormat, mFormat, mType);
    }

    void Texture::TextureParam(uint pname, uint param) const
    {
        glTexParameteri(mTarget, pname, param);
    }

    void Texture::InitFromFile(uint target, const char *file, bool mipmaps, TextureType type)
    {
        LoadFromFile(target, file, type);

        BindToSlot(EMPTY_SLOT);
        {
            if(mipmaps) {
                glGenerateMipmap(target);
                glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            }
            else {
                glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            }
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_REPEAT);
            const int maxMipLevel = int(std::log2(fmax(mSize.x, mSize.y))) - Setting<int>("maxMipmapLevelMod");
            glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, mipmaps ? maxMipLevel : 0);
            glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
        }
        UnBindFromSlot(EMPTY_SLOT);
    }

    void Texture::LoadFromFile(uint target, const char *file, TextureType type)
    {
        unsigned int imgid;
        ilGenImages(1, &imgid);
        ilBindImage(imgid);

        if(ilLoadImage((const ILstring)file) == 0) {
            LOG(ERROR) << "[Texture::InitFromFile] Failed to load texture: " << file;
            ilDeleteImages(1, &imgid);
            return;
        }
        if(!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE)) {
            LOG(ERROR) << "[Texture::InitFromFile] Failed to convert texture: " << file;
            ilDeleteImages(1, &imgid);
            return;
        }

        if(type == TextureType::DIFFUSE_MAP) {
            iluGammaCorrect(1.0f / Setting<float>("gamma"));
        }

        ILinfo ImageInfo;
        iluGetImageInfo(&ImageInfo);
        if(ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT) {
            iluFlipImage();
        }

        LOG(INFO) << "[Texture::LoadFromFile] Loaded Texture: " << file;

        Init();
        mSize = ivec2(ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
        mInternalFormat = GL_RGBA;
        mFormat = GL_RGBA;
        mType = GL_UNSIGNED_BYTE;

        BindToSlot(EMPTY_SLOT);
        glTexImage2D(target, 0, mInternalFormat, mSize.x, mSize.y, 0, mFormat, mType, ilGetData());
        UnBindFromSlot(EMPTY_SLOT);

        ilDeleteImages(1, &imgid);
    }

    void Texture::InitEmpty(uint target, uint internalFormat, ivec2 size, uint format, uint type)
    {
        mSize = size;
        mInternalFormat = internalFormat;
        mFormat = format;
        mType = type;

        Texture::Init();
        BindToSlot(0);
        {
            glTexImage2D(target,
                0,
                mInternalFormat,
                mSize.x,
                mSize.y,
                0,
                mFormat,
                mType,
                nullptr);

            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, 0);
            glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
        }
        UnBindFromSlot(0);
    }

    void Texture::GenerateMipmaps() const
    {
        BindToSlot(EMPTY_SLOT);
        {
            glGenerateMipmap(mTarget);
            glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            const int maxMipLevel = int(std::log2(fmax(mSize.x, mSize.y))) - Setting<int>("maxMipmapLevelMod");
            glTexParameteri(mTarget, GL_TEXTURE_MAX_LEVEL, maxMipLevel);
            glTexParameteri(mTarget, GL_TEXTURE_BASE_LEVEL, 0);
        }
        UnBindFromSlot(EMPTY_SLOT);
    }
}
