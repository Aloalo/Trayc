#include "Animation.h"

Animation::Animation(RTRTObject *object)
    : mObject(object)
{
}

Animation::~Animation(void)
{
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
