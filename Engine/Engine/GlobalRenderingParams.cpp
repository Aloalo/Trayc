#include <Engine/Engine/GlobalRenderingParams.h>

namespace engine
{
    int GetMRTIdx(TextureType type)
    {
        return type - TextureType::CT_MAT_TEXTURE_TYPES;
    }
}
