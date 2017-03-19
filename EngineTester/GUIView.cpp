
#include "GUIView.h"
#include <Engine/Engine/Game.h>
#include <Engine/Engine/AssetLoader.h>
#include <Engine/Utils/Setting.h>
#include <easylogging++.h>
#include <fstream>

using namespace engine;
using namespace glm;
using namespace std;

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
    if(e.repeat || e.type != SDL_KEYDOWN) {
        return;
    }

    const int skyboxCount = 3;
    static int skyboxIdx = 0;

    switch(e.keysym.sym)
    {
    case SDLK_ESCAPE:
    {
        Renderable::mIsActive = !Renderable::mIsActive;
        CameraHandler *camHandler = mGame->GetCameraHandler();
        camHandler->InputObserver::mActive = !camHandler->InputObserver::mActive;
        mGame->mInputHandler.SetCursorFree(!mGame->mInputHandler.IsCursorFree());
        break;
    }
    case SDLK_o:
        mGame->mRenderer.SetUsePBR(!mGame->mRenderer.UsePBR());
        LOG(INFO) << "[GUIView::KeyPress] PBR: " << mGame->mRenderer.UsePBR();
        break;
    case SDLK_b:
    {
        static int initialShadowsPasses = Setting<int>("softShadowsBlurPasses");
        const int currentShadowsPasses = Setting<int>("softShadowsBlurPasses");
        Setting<int>("softShadowsBlurPasses") = (currentShadowsPasses ? 0 : initialShadowsPasses);
        LOG(INFO) << "[GUIView::KeyPress] Shadow Blur Passes: " << Setting<int>("softShadowsBlurPasses");
        break;
    }
    case SDLK_F1:
    {
        // Load screenshot index
        ifstream fin(AssetLoader::Get().ResourcePath("screenshots.txt"));
        int idx = 0;
        if(fin.good()) {
            fin >> idx;
        }
        fin.close();
        ++idx;

        // Take screenshot
        int w, h;
        auto *pixels = mGame->mRenderer.TakeScreenshot(w, h);
        AssetLoader::Get().SavePicture(AssetLoader::Get().ResourcePath("screen" + to_string(idx) + ".png"), static_cast<void*>(pixels), w, h);
        delete[] pixels;

        // Save screenshot index
        ofstream fout(AssetLoader::Get().ResourcePath("screenshots.txt"));
        fout << idx;
        break;
    }
    case SDLK_UP:
    {
        skyboxIdx = (skyboxIdx + 1) % skyboxCount;
        mGame->mRenderer.LoadSkybox(skyboxIdx);
        break;
    }
    case SDLK_DOWN:
    {
        skyboxIdx = (skyboxIdx == 0 ? skyboxCount - 1 : skyboxIdx - 1);
        mGame->mRenderer.LoadSkybox(skyboxIdx);
        break;
    }
    default:
        break;
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
