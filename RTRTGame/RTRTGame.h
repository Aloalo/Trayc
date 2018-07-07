#ifndef RTRT_RTRT_GAME_H
#define RTRT_RTRT_GAME_H

#include "RTRTGamelike.h"
#include <Engine/Core/Updateable.h>
#include <Engine/Core/InputObserver.h>


class RTRTGame : public RTRTGamelike, public engine::Updateable, public engine::InputObserver
{
public:
    RTRTGame(engine::RayTraceRenderPass *rtPass, const engine::Camera *camera);

    virtual void Update(float dt) override;
};

#endif
