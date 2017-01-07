
#include "GUIView.h"
#include <Engine/Engine/Game.h>
#include <Engine/Engine/AssetLoader.h>
#include <Engine/Utils/Setting.h>
#include <easylogging++.h>

using namespace engine;
using namespace glm;

GUIView::GUIView(engine::Game *game)
    : mGame(game), mVA(GL_STATIC_DRAW)
{
    // Init half screen shading
    static const vec2 quad[4] =
    {
        vec2(1.0f, 1.0f),
        vec2(0.0f, 1.0f),
        vec2(0.0f,-1.0f),
        vec2(1.0f,-1.0f)
    };
    mVA.AddAttribute(VertexAttribDef(0, 2, GL_FLOAT, GL_FALSE));
    mVA.Init(4, 4);
    mVA.SetVertices(quad, 0, 4);

    mProgram.Init(AssetLoader::Get().ShaderPath("ColorToScreen").c_str());
    mProgram.Use();
    mProgram.SetUniform("color", vec4(0.0f, 0.0f, 0.0f, 0.5f));
    Program::Unbind();

    Renderable::mIsActive = false;
}

GUIView::~GUIView(void)
{
    mProgram.Destroy();
    mVA.Destroy();
}

void GUIView::KeyPress(const SDL_KeyboardEvent &e)
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

    if(e.keysym.sym == SDLK_o && e.type == SDL_KEYDOWN) {
        mGame->mRenderer.SetUsePBR(!mGame->mRenderer.UsePBR());
        LOG(INFO) << "[GUIView::KeyPress] PBR: " << mGame->mRenderer.UsePBR();
    }

    if(e.keysym.sym == SDLK_b && e.type == SDL_KEYDOWN) {
        static int initialShadowsPasses = Setting<int>("softShadowsBlurPasses");
        const int currentShadowsPasses = Setting<int>("softShadowsBlurPasses");

        Setting<int>("softShadowsBlurPasses") = (currentShadowsPasses ? 0 : initialShadowsPasses);

        LOG(INFO) << "[GUIView::KeyPress] Shadow Blur Passes: " << Setting<int>("softShadowsBlurPasses");
    }
}

void GUIView::Draw(const engine::RenderingContext &rContext) const
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    mProgram.Use();
    mVA.Render(GL_TRIANGLE_FAN);
    Program::Unbind();
    glDisable(GL_BLEND);
}
