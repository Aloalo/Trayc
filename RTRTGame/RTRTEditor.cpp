#include "RTRTEditor.h"
#include <Engine/Engine/RayTraceRenderPass.h>
#include <Engine/Engine/AssetLoader.h>
#include <easylogging++.h>
#include <jsonxx/jsonxx.h>
#include <fstream>
#include <map>
#include <cstdio>

using namespace engine;
using namespace glm;
using namespace std;
using namespace jsonxx;

RTRTEditor::RTRTEditor(RayTraceRenderPass *rtPass, const Camera *camera)
    : RTRTGamelike(rtPass, camera), mSelectedObject(nullptr)
{
}

void RTRTEditor::Update(float dt)
{
    mLevel.Update(dt);
}

void RTRTEditor::KeyPress(const SDL_KeyboardEvent &e)
{
    if (e.repeat || e.type != SDL_KEYDOWN)
        return;

    if (mSelectedObject) {
        const vec3 p = mSelectedObject->Position();
        const float dp = 1.0f;
        switch (e.keysym.sym)
        {
        case SDLK_i:
            mSelectedObject->SetPosition(vec3(p.x, p.y, p.z + dp));
            break;
        case SDLK_k:
            mSelectedObject->SetPosition(vec3(p.x, p.y, p.z - dp));
            break;
        case SDLK_j:
            mSelectedObject->SetPosition(vec3(p.x + dp, p.y, p.z));
            break;
        case SDLK_l:
            mSelectedObject->SetPosition(vec3(p.x - dp, p.y, p.z));
            break;
        case SDLK_o:
            mSelectedObject->SetPosition(vec3(p.x, p.y + dp, p.z));
            break;
        case SDLK_u:
            mSelectedObject->SetPosition(vec3(p.x, p.y - dp, p.z));
            break;
        case SDLK_m:
            SaveLevel();
            break;
        case SDLK_DELETE:
            if (mSelectedObject != nullptr) {
                RemoveObject(mSelectedObject);
                mSelectedObject = nullptr;
            }
            break;
        default:
            break;
        }
    }
}

void RTRTEditor::MouseButton(const SDL_MouseButtonEvent &e)
{
    if (e.button == SDL_BUTTON_LEFT && e.state == SDL_PRESSED) {
        const Ray r = {mCamera->mPosition, mCamera->GetDirection()};
        mSelectedObject = mLevel.RayCast(r);

        if (mSelectedObject) {
            LOG(INFO) << "Selected object: " << GetRTRTObjectTypeName(mSelectedObject->Type()) << ", ID: " << mSelectedObject->GetID();
        }
        else {
            LOG(INFO) << "No object selected";
        }
    }
}

void RTRTEditor::SaveLevel() const
{
    const string levelsPath = AssetLoader::Get().LevelsPath() + mLevel.mName;

    LOG(INFO) << "[RTRTEditor::SaveLevel] Saving level " << levelsPath;

    Array objects;

    for (const auto *o : mLevel.mObjects) {
        objects << o->GetJsonObject();
    }

    ofstream(levelsPath + ".json", ofstream::out) << objects.json();
}
