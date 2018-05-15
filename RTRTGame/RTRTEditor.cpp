#include "RTRTEditor.h"
#include <Engine/Engine/RayTraceRenderPass.h>
#include <Engine/Engine/AssetLoader.h>
#include <easylogging++.h>
#include <fstream>
#include <map>
#include <cstdio>

using namespace engine;
using namespace std;

RTRTEditor::RTRTEditor(RayTraceRenderPass *rtPass)
    : RTRTGamelike(rtPass)
{
}

void RTRTEditor::Update(float dt)
{
    mLevel.Update(dt);
}

void RTRTEditor::SetLevelFromPass()
{
    mLevel = RTRTLevel{};
    mLevel.SetLevelFromPass(mRTPass);
    mRTPass->CompileShaders();
}

void RTRTEditor::SaveLevel() const
{
    const string levelsPath = AssetLoader::Get().LevelsPath() + mLevel.mName;

    LOG(INFO) << "[RTRTEditor::SaveLevel] Saving level " << levelsPath;
 
    map<string, ofstream> counter;

    for (const auto *o : mLevel.mObjects) {

        if (counter.find(o->Type()) == counter.end()) {
            const string fname = levelsPath + "_" + o->Type();
            remove(fname.c_str());
            counter[o->Type()] = ofstream(fname, ios::app | ios::out | ios::binary);
        }

        int size = -1;
        const void *data = o->Data(size);

        counter[o->Type()].write(static_cast<const char*>(data), size);
    }
}
