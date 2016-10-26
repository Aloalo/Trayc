#include <Engine/GL/CubemapTexture.h>

#include <IL/il.h>
#include <IL/ilu.h>
#include <GL/glew.h>

#include <easylogging++.h>

using namespace std;
using namespace glm;

namespace engine
{
    CubemapTexture::uint CubemapTexture::mCubeSides[6] =
    {
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_X
    };

    CubemapTexture::CubemapTexture(void)
        : Texture(GL_TEXTURE_CUBE_MAP)
    {
    }

    void CubemapTexture::Init(const string files[6], TextureType type)
    {
        for(int i = 0; i < 6; ++i) {
            LoadFromFile(mCubeSides[i], files[i].c_str(), type);
        }
        GenerateMipmaps();
        TextureParam(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        TextureParam(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        TextureParam(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    void CubemapTexture::Resize(ivec2 size)
    {
        LOG(WARNING) << "[CubemapTexture::Resize] Unimplemented function" << endl;
    }
}
