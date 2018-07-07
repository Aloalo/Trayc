#include "RTRTGame.h"

using namespace engine;

RTRTGame::RTRTGame(RayTraceRenderPass *rtPass, const Camera *camera)
    : RTRTGamelike(rtPass, camera)
{
}

void RTRTGame::Update(float dt)
{
    const int ctAnms = static_cast<int>(mAnimations.size());
    for (int i = ctAnms - 1; i >= 0; --i) {
        if (mAnimations[i]->Update(dt)) {
            mAnimations.erase(mAnimations.begin() + i);
        }
    }

}
