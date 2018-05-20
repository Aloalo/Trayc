#ifndef RTRT_RTRT_EDITOR_H
#define RTRT_RTRT_EDITOR_H

#include <Engine/Core/Updateable.h>
#include <Engine/Core/InputObserver.h>
#include "RTRTGamelike.h"

class RTRTEditor : public RTRTGamelike, public engine::Updateable, public engine::InputObserver
{
public:
    RTRTEditor(engine::RayTraceRenderPass *rtPass, const engine::Camera *camera);

    virtual void Update(float dt) override;

    virtual void KeyPress(const SDL_KeyboardEvent &e) override;
    virtual void MouseButton(const SDL_MouseButtonEvent &e) override;

    void SetLevelFromPass();
    void SaveLevel() const;

private:
    RTRTObject *mSelectedObject;
};

#endif