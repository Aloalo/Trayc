#ifndef ET_LIGHT_HANDLER_H
#define ET_LIGHT_HANDLER_H

#include <Engine/Core/Updateable.h>
#include <Engine/Engine/Light.h>
#include <Engine/Utils/BSpline.h>

namespace engine
{
    class Game;
}

class LightHandler 
    : public engine::Updateable
{
public:
    LightHandler(engine::Game *game);

    virtual void Update(float dt) override;

private:
    engine::DirectionalLight mDLight;
    engine::PointLight mPLight;
    engine::CubicBSpline mBSpline;
};

#endif
