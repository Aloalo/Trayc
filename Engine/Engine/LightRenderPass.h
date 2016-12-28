
#ifndef EN_LIGHT_RENDER_PASS_H
#define EN_LIGHT_RENDER_PASS_H

#include <Engine/Engine/RenderPass.h>
#include <Engine/Engine/Light.h>
#include <Engine/Engine/TextureCombiner.h>

namespace engine
{
    __declspec(align(16)) class LightRenderPass : public RenderPass
    {
    public:
        void* operator new(size_t i)
        {
            return _mm_malloc(i, 16);
        }

        void operator delete(void* p)
        {
            _mm_free(p);
        }

        LightRenderPass(void);

        void CompileShaders();
        virtual void Init() override;
        virtual void Destroy() override;

        virtual void Render(const RenderingContext &rContext) const override;

        void SetLights(const std::vector<Light*> &lights);

    private:

        std::vector<Light*> mLights;
        TextureCombiner mLightCombiners[Light::Type::CT_LIGHT_TYPES];
    };
}

#endif
