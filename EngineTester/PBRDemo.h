#ifndef ET_PBR_DEMO_H
#define ET_PBR_DEMO_H

#include <Engine/Core/InputObserver.h>
#include <Engine/Engine/Game.h>
#include <Engine/Geometry/Scene.h>
#include <Engine/Engine/Light.h>
#include <Engine/Utils/BSpline.h>

class PBRDemo
    : public engine::InputObserver
{
public:
    PBRDemo(void);

    void Init(engine::RasterizedGame &game, engine::Scene &scene);
    virtual void KeyPress(const SDL_KeyboardEvent &e) override;
    virtual void MouseMotion(const SDL_MouseMotionEvent &e) override;
    virtual void MouseButton(const SDL_MouseButtonEvent &e) override;

private:
    engine::GlobalLight mLight;
};

#endif
