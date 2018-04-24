#include "RTRTGamelike.h"
#include <Engine/Engine/AssetLoader.h>
#include <Engine/Engine/RayTraceRenderPass.h>
#include <easylogging++.h>
#include <fstream>
#include <vector>

using namespace std;
using namespace engine;

RTRTGamelike::RTRTGamelike(RayTraceRenderPass *rtPass)
    : mRTPass(rtPass)
{
}

void RTRTGamelike::LoadLevel(const string &name)
{
    const string levelsPath = AssetLoader::Get().LevelsPath() + name;

    LOG(INFO) << "[RTRTEditor::LoadLevel] Loading level " << levelsPath;

    mLevel = RTRTLevel();
    for (const auto &type : OBJECT_TYPES) {
        const auto fname = levelsPath + "_" + type;
        ifstream infile(fname, ios::ate | ios::binary);
        const streamsize fsize = infile.tellg();
        infile.seekg(0, infile.beg);

        if (infile.good()) {
            if (type == "RTSphere") {
                mRTPass->mSpheres.clear();
                mRTPass->mSpheres.resize(fsize / sizeof(RTSphere));
                infile.read(reinterpret_cast<char*>(mRTPass->mSpheres.data()), fsize);
            }
            else if (type == "RTRectangle") {
                mRTPass->mRectangles.clear();
                mRTPass->mRectangles.resize(fsize / sizeof(RTRectangle));
                infile.read(reinterpret_cast<char*>(mRTPass->mRectangles.data()), fsize);
            }
            else if (type == "RTBox") {
                mRTPass->mBoxes.clear();
                mRTPass->mBoxes.resize(fsize / sizeof(RTBox));
                infile.read(reinterpret_cast<char*>(mRTPass->mBoxes.data()), fsize);
            }
            else if (type == "RTLight") {
                mRTPass->mLights.clear();
                mRTPass->mLights.resize(fsize / sizeof(RTLight));
                infile.read(reinterpret_cast<char*>(mRTPass->mLights.data()), fsize);
            }
        }
    }
}
