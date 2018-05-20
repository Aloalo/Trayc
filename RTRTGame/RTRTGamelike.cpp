#include "RTRTGamelike.h"
#include <Engine/Core/Camera.h>
#include <Engine/Engine/AssetLoader.h>
#include <Engine/Engine/RayTraceRenderPass.h>
#include <easylogging++.h>
#include <fstream>
#include <vector>

using namespace std;
using namespace engine;

RTRTGamelike::RTRTGamelike(RayTraceRenderPass *rtPass, const Camera *camera)
    : mRTPass(rtPass), mCamera(camera)
{
}

void RTRTGamelike::LoadLevel(const string &name)
{
    const string levelsPath = AssetLoader::Get().LevelsPath() + name;

    LOG(INFO) << "[RTRTEditor::LoadLevel] Loading level " << levelsPath;

    mLevel = RTRTLevel();
    for (RTRTObjectType type : RTRT_OBJECT_TYPES) {
        const auto fname = levelsPath + "_" + GetRTRTObjectTypeName(type);
        ifstream infile(fname, ios::ate | ios::binary);
        const streamsize fsize = infile.tellg();
        infile.seekg(0, infile.beg);

        if (!infile.good()) {
            throw exception(("File open failed: " + fname).c_str());
        }

        switch (type)
        {
        case RTRTObjectType::RTRTSphere:
        {
            mRTPass->mSpheres.clear();
            mRTPass->mSpheres.resize(fsize / sizeof(RTSphere));
            infile.read(reinterpret_cast<char*>(mRTPass->mSpheres.data()), fsize);
            break;
        }
        case RTRTObjectType::RTRTRectangle:
        {
            mRTPass->mRectangles.clear();
            mRTPass->mRectangles.resize(fsize / sizeof(RTRectangle));
            infile.read(reinterpret_cast<char*>(mRTPass->mRectangles.data()), fsize);
            break;
        }
        case RTRTObjectType::RTRTBox:
        {
            mRTPass->mBoxes.clear();
            mRTPass->mBoxes.resize(fsize / sizeof(RTBox));
            infile.read(reinterpret_cast<char*>(mRTPass->mBoxes.data()), fsize);
            break;
        }
        case RTRTObjectType::RTRTLight:
        {
            mRTPass->mLights.clear();
            mRTPass->mLights.resize(fsize / sizeof(RTLight));
            infile.read(reinterpret_cast<char*>(mRTPass->mLights.data()), fsize);
            break;
        }
        }
    }

    mLevel.SetLevelFromPass(mRTPass);
    mRTPass->CompileShaders();
}
