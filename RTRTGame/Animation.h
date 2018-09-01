#ifndef RTRT_ANIMATION_H
#define RTRT_ANIMATION_H

#include "RTRTObjects.h"

class RTRTLevel;

class Animation
{
public:
    enum Type
    {
        YAW_ROTATION = 0,
        CT_ANIMATION_TYPES
    };

    Animation(RTRTObject *object);
    virtual  ~Animation(void);

    virtual bool Update(float dt) = 0;
    virtual Type GetType() const = 0;

    static std::vector<Animation*> LoadAnimations(RTRTLevel *level, const std::string &fname);

protected:
    RTRTObject *mObject;
};

class YawRotationAnimation : public Animation
{
public:
    YawRotationAnimation(RTRTObject *object, float angularVelocity);
    virtual bool Update(float dt) override;
    virtual Type GetType() const override;

private:
    float mAngularVelocity;
};

#endif