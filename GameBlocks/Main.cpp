/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Engine.h>
#include <Engine/Core/DefaultCameraHandler.h>
#include <Engine/Engine/TextureCombiner.h>

using namespace engine;
using namespace std;
using namespace glm;

class TestRenderTexture : public Renderable
{
public:
    TestRenderTexture(void)
    {
        mTexRenderer.Init("../Resources/Shaders/C_TexToScreen");
        mTex.Init("../Resources/Textures/ErrorTex.png");
    }

    virtual void Draw(const RenderingContext &rContext) const override
    {
        mTexRenderer.SetTexture(0, mTex);
        mTexRenderer.Prog().Use();
        mTexRenderer.Prog().SetUniform("tex", 0);
        mTexRenderer.Draw();
    }

private:
    Texture2D mTex;
    TextureCombiner mTexRenderer;
};

DefaultCameraHandler ConstructCameraHandler(ivec2 ss, float FOV, float farDist)
{
    const float nearDist = 0.1f;
    const vec3 cameraPos(40.0f);
    const Camera camera(cameraPos, float(ss.x) / float(ss.y), FOV, nearDist, farDist);

    const float moveSpeed = 0.006f;
    const float rotationSpeed = 6.0f;
    const float springiness = 15.0f;

    return DefaultCameraHandler(camera, moveSpeed, rotationSpeed, springiness);
}

int main(int argc, char *argv[])
{
    const ivec2 SSize(1280, 720);
    const float timeStep = 1.0f / 60.0f;
    //Init Camera handler
    DefaultCameraHandler camHandler(ConstructCameraHandler(SSize, 60.0f, 1000.0f));
    //Init Scene
    Scene scene(timeStep);
    scene.Init(&camHandler, argv[0], "Test", SSize.x, SSize.y);
    scene.mSDLHandler.VsyncMode(1);
    scene.mRenderer.SetClearColor(vec4(0.3f, 0.3f, 0.3f, 1.0f));

    TestRenderTexture test;

    scene.mRenderer.AddRenderable(&test);

    scene.GameLoop();
    return 0;
}
