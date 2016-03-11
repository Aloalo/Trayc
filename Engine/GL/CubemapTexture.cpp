#include <Engine/GL/CubemapTexture.h>

#include <IL/il.h>
#include <IL/ilu.h>
#include <GL/glew.h>

#include <easylogging++.h>

using namespace std;
using namespace glm;

namespace engine
{
    CubemapTexture::CubemapTexture(void)
        : Texture(GL_TEXTURE_CUBE_MAP)
    {
    }

    void CubemapTexture::Resize(ivec2 size)
    {
    }
}
