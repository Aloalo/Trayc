#ifndef ET_HEAD_DEMO_H
#define ET_HEAD_DEMO_H

#include <Engine/Core/InputObserver.h>
#include <Engine/Engine/Game.h>
#include <Engine/Geometry/Scene.h>
#include <Engine/Engine/Light.h>
#include <Engine/Utils/BSpline.h>

class HeadDemo
    : public engine::InputObserver
{
public:
    explicit HeadDemo(float scale);

    void Init(engine::Game &game, engine::Scene &scene);
    virtual void MouseMotion(const SDL_MouseMotionEvent &e) override;
    virtual void MouseButton(const SDL_MouseButtonEvent &e) override;

private:
    bool mMouseDown;
    float mRotY;
    float mScale;

    engine::Object3D* mHead;
    engine::GlobalLight mLight;
};

#endif
