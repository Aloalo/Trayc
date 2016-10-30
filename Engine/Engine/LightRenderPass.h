
#ifndef EN_LIGHT_RENDER_PASS_H
#define EN_LIGHT_RENDER_PASS_H

#include <Engine/Engine/RenderPass.h>
#include <Engine/Engine/Light.h>
#include <Engine/Engine/TextureCombiner.h>

namespace engine
{
    class LightRenderPass : public RenderPass
    {
    public:
        LightRenderPass(void);

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
