
#include "GUIView.h"
#include <Engine/Engine/Game.h>
#include <Engine/Engine/AssetLoader.h>
#include <Engine/Engine/RayTraceRenderPass.h>
#include <Engine/Engine/BackBufferRenderPass.h>
#include <easylogging++.h>
#include <fstream>

using namespace engine;
using namespace glm;
using namespace std;

GUIView::GUIView(engine::RayTracedGame *game)
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
    case SDLK_F1:
    {
        LOG(INFO) << "Taking screenshot";

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
    case SDLK_F2:
    {
        LOG(INFO) << "Recompiling shaders";

        RayTraceRenderPass *rtPass = dynamic_cast<RayTraceRenderPass*>(mGame->mRenderer.GetRenderPass("rtPass"));
        rtPass->CompileShaders();

        BackBufferRenderPass *bbPass = dynamic_cast<BackBufferRenderPass*>(mGame->mRenderer.GetRenderPass("bbPass"));
        bbPass->CompileShaders();
        break;
    }
    case SDLK_c:
    {
        LOG(INFO) << "Toggling checkerboarding";

        RayTraceRenderPass *rtPass = dynamic_cast<RayTraceRenderPass*>(mGame->mRenderer.GetRenderPass("rtPass"));
        rtPass->ToggleCheckerboarding();
        break;
    }
    default:
        break;
    }
}

void GUIView::Draw(const engine::RenderingContext &rContext) const
{
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    //mProgram.Use();
    //mVA.Render(GL_TRIANGLE_FAN);
    //Program::Unbind();
    //glDisable(GL_BLEND);
}
