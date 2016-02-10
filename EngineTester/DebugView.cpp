
#include "DebugView.h"
#include <Engine/Engine/Game.h>

using namespace engine;

DebugView::DebugView(engine::Game *game)
    : mGame(game)
{
}

void DebugView::KeyPress(const SDL_KeyboardEvent &e)
{
    if(e.repeat) {
        return;
    }

    if(e.keysym.sym == SDLK_ESCAPE && e.type == SDL_KEYDOWN) {
        Renderable::mIsActive = !Renderable::mIsActive;

        CameraHandler *camHandler = mGame->GetCameraHandler();
        camHandler->InputObserver::mActive = !camHandler->InputObserver::mActive;

        mGame->mInputHandler.SetCursorFree(!mGame->mInputHandler.IsCursorFree());
    }
}

void DebugView::Draw(const engine::RenderingContext &rContext) const
{
}
