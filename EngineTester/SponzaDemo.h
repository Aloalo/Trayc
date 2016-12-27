#ifndef ET_SPONZA_DEMO_H
#define ET_SPONZA_DEMO_H

#include <Engine/Core/Updateable.h>
#include <Engine/Core/InputObserver.h>
#include <Engine/Engine/Light.h>
#include <Engine/Utils/BSpline.h>

namespace engine
{
    class Scene;
    struct Object3D;
}

class SponzaDemo 
    : public engine::Updateable, public engine::InputObserver
{
public:
    SponzaDemo(void);

    void Init(engine::Scene *scene);
    virtual void Update(float dt) override;
    virtual void KeyPress(const SDL_KeyboardEvent &e) override;

private:
    bool mPause;

    engine::GlobalLight mGLight;

    engine::PointLight mPLight;
    engine::CubicBSpline mBSpline;
    engine::Object3D *mPLightObj;

    engine::SpotLight mSLight;
};

#endif
