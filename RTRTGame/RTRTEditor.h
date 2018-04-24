#ifndef RTRT_RTRT_EDITOR_H
#define RTRT_RTRT_EDITOR_H

#include <Engine/Core/Updateable.h>
#include "RTRTGamelike.h"

class RTRTEditor : public engine::Updateable, public RTRTGamelike
{
public:
    RTRTEditor(engine::RayTraceRenderPass *rtPass);

    virtual void Update(float dt) override;

    void SetLevelFromPass();
    void SaveLevel() const;
};

#endif