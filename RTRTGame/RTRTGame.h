#ifndef RTRT_RTRT_GAME_H
#define RTRT_RTRT_GAME_H

#include <Engine/Core/Updateable.h>
#include "RTRTLevel.h"

class RTRTGame : public engine::Updateable
{
public:
    virtual void Update(float dt) override;

private:
    RTRTLevel mLevel;
};

#endif