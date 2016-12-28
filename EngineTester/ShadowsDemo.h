#ifndef ET_SHADOWS_DEMO_H
#define ET_SHADOWS_DEMO_H

#include <Engine/Core/InputObserver.h>
#include <Engine/Engine/Game.h>
#include <Engine/Geometry/Scene.h>
#include <Engine/Engine/Light.h>
#include <Engine/Utils/BSpline.h>

class ShadowsDemo
    : public engine::InputObserver
{
public:
    ShadowsDemo(void);

    void Init(engine::Game &game, engine::Scene &scene, const std::string &model);
    virtual void MouseMotion(const SDL_MouseMotionEvent &e) override;
    virtual void MouseButton(const SDL_MouseButtonEvent &e) override;

private:
    glm::vec3 mLightLookAt;
    glm::vec3 mLightStartPos;
    float mRotX;
    int mCurrSceneIdx;
    bool mMouseDown;

    engine::Game *mGame;
    engine::Object3D *mPLightObj;
    engine::SpotLight mLight;
    engine::AmbientLight mAmbientLight;
};

#endif
