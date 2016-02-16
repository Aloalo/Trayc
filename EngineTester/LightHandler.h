#ifndef ET_LIGHT_HANDLER_H
#define ET_LIGHT_HANDLER_H

#include <Engine/Core/Updateable.h>
#include <Engine/Engine/Light.h>
#include <Engine/Utils/BSpline.h>

namespace engine
{
    class Scene;
    struct Object3D;
}

class LightHandler 
    : public engine::Updateable
{
public:
    LightHandler(engine::Scene *scene);

    virtual void Update(float dt) override;

private:
    engine::AmbientLight mALight;
    engine::DirectionalLight mDLight;

    engine::PointLight mPLight;
    engine::CubicBSpline mBSpline;
    engine::Object3D *mPLightObj;

    engine::SpotLight mSLight;
};

#endif
