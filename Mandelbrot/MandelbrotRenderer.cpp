#include "MandelbrotRenderer.h"

using namespace glm;
using namespace engine;

MandelbrotRenderer::MandelbrotRenderer(vec2 windowSize) :
    mQuad(GL_STATIC_DRAW), mMovingView(false)
{
    mState.mTopLeftCoord = vec2(2.0f, 1.0f);
    mState.mWindowSize = windowSize;
    mState.mScale = 512.0f;
    mState.mLimit = 400.0f;
    mState.mAA = 0;
    mState.mPrecision = 0;

    mMandelProgF.Init(VertexShader("Shaders/mandelbrot"), FragmentShader("Shaders/mandelbrotF"), "mandelbrotF");
    mMandelProgD.Init(VertexShader("Shaders/mandelbrot"), FragmentShader("Shaders/mandelbrotD"), "mandelbrotD");

    mCurrentProg = &mMandelProgF;
    ApplyState();

    glViewport(0, 0, GLsizei(windowSize.x), GLsizei(windowSize.y));

    static const vec2 quad[4] =
    {
        vec2(-2.0f, 1.0f),
        vec2(1.0f, 1.0f),
        vec2(-2.0f, -1.0f),
        vec2(1.0f, -1.0f)
    };
    mQuad.AddAttribute(VertexAttribDef(0, 2, GL_FLOAT, GL_FALSE));
    mQuad.Init(4, 4);
    mQuad.SetVertices(quad, 0, 4);
}

MandelbrotRenderer::~MandelbrotRenderer(void)
{
    mMandelProgD.Delete();
    mMandelProgF.Delete();
}

vec2 lpo;
void MandelbrotRenderer::KeyPress(const SDL_KeyboardEvent &e)
{
    if(e.type == SDL_KEYDOWN)
    {
        switch(e.keysym.sym)
        {
        case SDLK_q:
            mState.mTopLeftCoord += lpo * (1.0f / (mState.mScale / 0.9f) - 1.0f / mState.mScale);
            mState.mScale /= 0.9f;
            break;
        case SDLK_w:
            mState.mTopLeftCoord += lpo * (1.0f / (mState.mScale * 0.9f) - 1.0f / mState.mScale);
            mState.mScale *= 0.9f;
            break;
        case SDLK_1:
            mState.mLimit = 402.0f;
            break;
        case SDLK_2:
            mState.mLimit = float(1 << 12) + 2.0f;
            break;
        case SDLK_3:
            mState.mAA = !mState.mAA;
            break;
        case SDLK_4:
        {
            mState.mPrecision = !mState.mPrecision;
            mCurrentProg = (mState.mPrecision == 0 ? &mMandelProgF : &mMandelProgD);
            break;
        }
        default:
            break;
        }
    }
}

void MandelbrotRenderer::MouseMotion(const SDL_MouseMotionEvent &e)
{
    static vec2 lp;
    vec2 p(e.x, e.y);
    lpo = p;
    p /= mState.mScale;
    p -= mState.mTopLeftCoord;
    const vec2 d = p - lp;
    if(mMovingView)
    {
        mState.mTopLeftCoord += d;
        p -= d;
    }
    lp = p;
}

void MandelbrotRenderer::MouseButton(const SDL_MouseButtonEvent &e)
{
    if(e.button == SDL_BUTTON_LEFT || e.button == SDL_BUTTON_MIDDLE)
        mMovingView = (e.state == SDL_PRESSED);
}

void MandelbrotRenderer::MouseWheel(const SDL_MouseWheelEvent &e)
{
    if(e.y > 0)
    {
        mState.mTopLeftCoord += lpo * (1.0f / (mState.mScale / 0.9f) - 1.0f / mState.mScale);
        mState.mScale /= 0.9f;
    }
    else if(e.y < 0)
    {
        mState.mTopLeftCoord += lpo * (1.0f / (mState.mScale * 0.9f) - 1.0f / mState.mScale);
        mState.mScale *= 0.9f;
    }
}

void MandelbrotRenderer::WindowEvent(const SDL_WindowEvent & e)
{
    if(e.event == SDL_WINDOWEVENT_RESIZED)
    {
        glViewport(0, 0, e.data1, e.data2);
        mState.mWindowSize = vec2(e.data1, e.data2);
    }
}

void MandelbrotRenderer::Draw(const engine::RenderingContext &rContext) const
{
    ApplyState();
    mQuad.Render(GL_TRIANGLE_STRIP);
}

void MandelbrotRenderer::ApplyState() const
{
    mCurrentProg->Use();
    mCurrentProg->SetUniform("translation", mState.mTopLeftCoord);
    mCurrentProg->SetUniform("scale", mState.mScale);
    mCurrentProg->SetUniform("windowSize", mState.mWindowSize);
    mCurrentProg->SetUniform("limit", mState.mLimit);
    mCurrentProg->SetUniform("aa", mState.mAA);
}
