#ifndef RTRT_ANIMATION_H
#define RTRT_ANIMATION_H

#include "RTRTObjects.h"

class Animation
{
public:
    Animation(RTRTObject *object);
    virtual  ~Animation(void);

    virtual bool Update(float dt) = 0;

protected:
    RTRTObject *mObject;
};

class YawRotationAnimation : public Animation
{
public:
    YawRotationAnimation(RTRTObject *object, float angularVelocity);
    virtual bool Update(float dt) override;

private:
    float mAngularVelocity;
};

#endif