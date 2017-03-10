#include "HeadDemo.h"
#include <Engine/Geometry/GeometryMeshes.h>
#include <Engine/Engine/AssetLoader.h>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>


using namespace engine;
using namespace glm;
using namespace std;

HeadDemo::HeadDemo(float scale)
    : mMouseDown(false), mRotY(0.0f), mScale(scale), mHead(nullptr),
    mLight(vec3(0.09f), vec3(2.0f), true, vec3(1.0f, 0.0f, 1.0f))
{
}

void HeadDemo::Init(Game &game, Scene &scene)
{
    // Init Light
    scene.mLights.push_back(&mLight);
    game.mRenderer.SetScene(&scene);

    mHead = &scene.mObjects3D[0];

    game.mInputHandler.AddEventListener(this);
}

void HeadDemo::MouseButton(const SDL_MouseButtonEvent &e)
{
    if(e.button == SDL_BUTTON_RIGHT) {
        mMouseDown = (e.state == SDL_PRESSED);
    }
}

void HeadDemo::MouseMotion(const SDL_MouseMotionEvent &e)
{
    if(mMouseDown) {
        static const mat4 I(1.0f);
        mRotY += float(e.xrel) / 80.0f;

        mHead->SetTransform(scale(I, vec3(mScale)));
        mHead->SetTransform(rotate(mHead->GetTransform(), mRotY, vec3(0.0f, 1.0f, 0.0f)));
    }
}
