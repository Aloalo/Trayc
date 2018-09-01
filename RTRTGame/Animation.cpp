#include "Animation.h"
#include <jsonxx/jsonxx.h>
#include <fstream>
#include "RTRTLevel.h"

using namespace std;
using namespace jsonxx;

Animation::Animation(RTRTObject *object)
    : mObject(object)
{
}

Animation::~Animation(void)
{
}

static inline Array LoadArray(const string &path)
{
    ifstream jsonFile(path);
    const string jsonStr((istreambuf_iterator<char>(jsonFile)), istreambuf_iterator<char>());
    Array ret;
    ret.parse(jsonStr);
    return ret;
}

static inline Animation* AnimationFactory(RTRTLevel *level, const Object &obj)
{
    const int objectID = static_cast<int>(obj.get<Number>("objectID"));
    const Animation::Type type = static_cast<Animation::Type>(static_cast<int>(obj.get<Number>("type")));
    RTRTObject *object = level->GetRTRTObject(objectID);

    switch (type)
    {
    case Animation::Type::YAW_ROTATION:
        return new YawRotationAnimation(object, static_cast<float>(obj.get<Number>("angularVelocity")));
    default:
        throw exception(("Unknown Animation type: " + to_string(type)).c_str());
    }
}

vector<Animation*> Animation::LoadAnimations(RTRTLevel *level, const string &fname)
{
    std::vector<Animation*> ret;

    const Array animations = LoadArray(fname);
    const int ctAnimations = static_cast<int>(animations.size());

    for (int i = 0; i < ctAnimations; ++i) {
        const Object &obj = animations.get<Object>(i);
        ret.push_back(AnimationFactory(level, obj));
    }

    return ret;
}

YawRotationAnimation::YawRotationAnimation(RTRTObject *object, float angularVelocity)
    : Animation(object), mAngularVelocity(angularVelocity)
{
}

bool YawRotationAnimation::Update(float dt)
{
    const float dYaw = dt * mAngularVelocity;
    mObject->RotateYaw(dYaw);
    return false;
}

Animation::Type YawRotationAnimation::GetType() const
{
    return YAW_ROTATION;
}
