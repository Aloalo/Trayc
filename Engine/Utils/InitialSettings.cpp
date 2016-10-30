
#include <Engine/Utils/InitialSettings.h>
#include <Engine/Engine/AssetLoader.h>
#include <fstream>
#include <easylogging++.h>

using namespace std;
using namespace jsonxx;

namespace engine
{
    InitialSettings::InitialSettings()
    {
        const string settingsFile = AssetLoader::Get().ResourcePath("Settings.json");
        ifstream jsonSettings(settingsFile);
        const string jsonSettingsStr((istreambuf_iterator<char>(jsonSettings)), istreambuf_iterator<char>());
        mSettings.parse(jsonSettingsStr);
    }
}
