#ifndef ET_PBR_MATERIAL_DEMO_H
#define ET_PBR_MATERIAL_DEMO_H

#include <Engine/Core/InputObserver.h>
#include <Engine/Engine/Game.h>
#include <Engine/Geometry/Scene.h>
#include <Engine/Engine/Light.h>
#include <Engine/Utils/BSpline.h>

class PBRMaterialDemo
    : public engine::InputObserver
{
public:
    PBRMaterialDemo(void);

    void Init(engine::RasterizedGame &game, engine::Scene &scene);
    virtual void KeyPress(const SDL_KeyboardEvent &e) override;
    virtual void MouseMotion(const SDL_MouseMotionEvent &e) override;
    virtual void MouseButton(const SDL_MouseButtonEvent &e) override;

private:
    bool mMouseDown;
    int mCurrObjIdx;
    float mRotX, mRotY;

    std::vector<engine::Object3D*> mObjects;
    engine::GlobalLight mLight;
};

#endif
