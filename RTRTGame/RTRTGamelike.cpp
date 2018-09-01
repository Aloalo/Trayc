#include "RTRTGamelike.h"
#include <Engine/Core/Camera.h>
#include <Engine/Engine/AssetLoader.h>
#include <Engine/Engine/RayTraceRenderPass.h>
#include <easylogging++.h>
#include <jsonxx/jsonxx.h>
#include <fstream>
#include <vector>

using namespace std;
using namespace engine;
using namespace jsonxx;

RTRTGamelike::RTRTGamelike(RayTraceRenderPass *rtPass, const Camera *camera)
    : mRTPass(rtPass), mCamera(camera)
{
}

RTRTGamelike::~RTRTGamelike(void)
{
    for (Animation *anm : mAnimations) {
        delete anm;
    }
}

void RTRTGamelike::SetLevelFromPass()
{
    mLevel = RTRTLevel{};
    mLevel.SetLevelFromPass(mRTPass);
    mRTPass->CompileShaders();
}

static inline Array LoadArray(const string &path)
{
    ifstream jsonFile(path);
    const string jsonStr((istreambuf_iterator<char>(jsonFile)), istreambuf_iterator<char>());
    Array ret;
    ret.parse(jsonStr);
    return ret;
}

void RTRTGamelike::LoadLevel(const string &name)
{
    const string levelPath = AssetLoader::Get().LevelsPath() + name;

    LOG(INFO) << "[RTRTEditor::LoadLevel] Loading level " << levelPath;

    mLevel = RTRTLevel();
    mRTPass->Clear();

    const Array objects = LoadArray(levelPath + ".json");
    const int ctObjects = static_cast<int>(objects.size());
    int dummy = -1;

    for (int i = 0; i < ctObjects; ++i)
    {
        const Object &obj = objects.get<Object>(i);
        RTRTObject *rtrtObj = RTRTObjectFactory(obj);

        switch (rtrtObj->Type())
        {
        case RTRTObjectType::RTRTSphere:
        {
            mRTPass->mSpheres.push_back(*reinterpret_cast<const RTSphere*>(rtrtObj->Data(dummy)));
            break;
        }
        case RTRTObjectType::RTRTRectangle:
        {
            mRTPass->mRectangles.push_back(*reinterpret_cast<const RTRectangle*>(rtrtObj->Data(dummy)));
            break;
        }
        case RTRTObjectType::RTRTBox:
        {
            mRTPass->mBoxes.push_back(*reinterpret_cast<const RTBox*>(rtrtObj->Data(dummy)));
            break;
        }
        case RTRTObjectType::RTRTLight:
        {
            mRTPass->mLights.push_back(*reinterpret_cast<const RTLight*>(rtrtObj->Data(dummy)));
            break;
        }
        }
    }

    mLevel.SetLevelFromPass(mRTPass);
    mRTPass->CompileShaders();

    mAnimations = Animation::LoadAnimations(&mLevel, levelPath + "_animations.json");
}

void RTRTGamelike::RemoveObject(RTRTObject *object)
{
    mLevel.mObjects.erase(std::remove(mLevel.mObjects.begin(), mLevel.mObjects.end(), object), mLevel.mObjects.end());
    // TODO(jure): remove from rendering
}
