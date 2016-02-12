
#ifndef EN_LIGHT_RENDER_PASS_H
#define EN_LIGHT_RENDER_PASS_H

#include <Engine/Engine/RenderPass.h>
#include <Engine/Engine/Light.h>
#include <Engine/GL/Program.h>
#include <Engine/GL/Program.h>

namespace engine
{
    class LightRenderPass : public RenderPass
    {
    public:
        LightRenderPass(void);

        virtual void Init() override;
        virtual void Destroy() override;

        virtual void BeginRender() const override;
        virtual void EndRender() const override;

        virtual void Render(const std::vector<RenderPass*> &renderPasses, const RenderingContext &rContext) const override;

        void AddLight(Light *light);
        void RemoveLight(Light *light);

    private:
        std::vector<Light*> mLights;
        Program mLightPrograms[LightType::CT_LIGHT_TYPES];
    };
}

#endif
