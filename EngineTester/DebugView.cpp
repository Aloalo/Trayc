
#include "DebugView.h"
#include <Engine/Engine/Renderer.h>
#include <Engine/Engine/ShadowProjectionRenderPass.h>
#include <Engine/Engine/ShadowRenderPass.h>
#include <Engine/Engine/DebugDraw.h>
#include <Engine/Utils/Setting.h>

using namespace engine;
using namespace glm;

DebugView::DebugView(const engine::Renderer *renderer, float nearDist, float farDist)
    : mRenderer(renderer), mTexType(TextureType::G_ALBEDO_TEXTURE), mNearDist(nearDist), mFarDist(farDist), mDrawGloss(false)
{
    Renderable::mIsActive = false;
}

void DebugView::KeyPress(const SDL_KeyboardEvent &e)
{
    if(e.repeat || e.type != SDL_KEYDOWN) {
        return;
    }

    switch(e.keysym.sym)
    {
    case SDLK_1:
        Renderable::mIsActive = false;
        break;
    case SDLK_2:
        Renderable::mIsActive = true;
        mTexType = TextureType::G_DEPTH_TEXTURE;
        break;
    case SDLK_3:
        Renderable::mIsActive = true;
        mTexType = TextureType::G_NORMAL_TEXTURE;
        break;
    case SDLK_4:
        Renderable::mIsActive = true;
        mDrawGloss = false;
        mTexType = TextureType::G_SPEC_GLOSS_TEXTURE;
        break;
    case SDLK_5:
        Renderable::mIsActive = true;
        mDrawGloss = true;
        mTexType = TextureType::G_SPEC_GLOSS_TEXTURE;
        break;
    case SDLK_6:
        Renderable::mIsActive = true;
        mTexType = TextureType::G_ALBEDO_TEXTURE;
        break;
    case SDLK_7:
        Renderable::mIsActive = true;
        mTexType = TextureType::S_SHADOWMAP;
        break;
    case SDLK_8:
        Renderable::mIsActive = true;
        mTexType = TextureType::S_SHADOWPROJECTION;
        break;
    default:
        break;
    }

}

void DebugView::Draw(const engine::RenderingContext &rContext) const
{
    if(mTexType == TextureType::S_SHADOWMAP) {
        
        const ShadowRenderPass *shadowPass = dynamic_cast<const ShadowRenderPass*>(mRenderer->GetRenderPass("shadowPass"));
        const Texture2D &tex = shadowPass->GetShadowmap(1);
        DebugDraw::Get().DrawDepth(tex, 0.0f, -1.0f);
        return;
    }

    if(mTexType == TextureType::S_SHADOWPROJECTION) {
        const ShadowProjectionRenderPass *shadowPass = dynamic_cast<const ShadowProjectionRenderPass*>(mRenderer->GetRenderPass("shadowProjectionPass"));
        const Texture2D &tex = shadowPass->GetProjectedShadowmap(1);
        DebugDraw::Get().DrawTexture(tex, vec3(1.0f));
        return;
    }

    const RenderPass *gPass = mRenderer->GetRenderPass("gPass");
    const Texture2D &tex = gPass->GetDstBuffer().GetAttachment(GetMRTIdx(mTexType));

    switch(mTexType)
    {
    case TextureType::G_DEPTH_TEXTURE:
        DebugDraw::Get().DrawDepth(tex, mNearDist, mFarDist);
        break;
    case TextureType::G_NORMAL_TEXTURE:
        DebugDraw::Get().DrawNormal(tex);
        break;
    case TextureType::G_ALBEDO_TEXTURE:
        DebugDraw::Get().DrawTexture(tex, vec3(Setting<float>("gamma")));
        break;
    case TextureType::G_SPEC_GLOSS_TEXTURE:
        if(mDrawGloss) {
            DebugDraw::Get().DrawGloss(tex);
        }
        else {
            DebugDraw::Get().DrawTexture(tex, vec3(1.0f));
        }
        break;
    default:
        break;
    }
}
