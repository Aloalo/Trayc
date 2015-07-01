/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "FunctionTracer.h"
#include "UserSettings.h"
#include <Engine/Utils/Utilities.h>
#include <Engine/Utils/MathFunctions.h>
#include "TmpHandlers.h"

using namespace std;
using namespace glm;
using namespace engine;

FunctionTracer::FunctionTracer(void)
    : FunctionDrawer("Shaders/tracing")
{
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);

    static const GLfloat quad[] = 
    { 
        -1.0f, -1.0f,
        1.0f, -1.0f,
        1.0f, 1.0f,
        -1.0f, 1.0f
    };

    glBindVertexArray(mVAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    }
    glBindVertexArray(0);

}

FunctionTracer::~FunctionTracer(void)
{
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
}

void FunctionTracer::SetFunction(const std::string &F, const std::string &Fx, const std::string &Fy)
{
    FunctionDrawer::SetFunction(F, Fx, Fy);
}

void FunctionTracer::ApplyFunction()
{
    string G(mF);
    StringReplace(G, "exp", "eee");
    StringReplace(G, "x", "(E.x + L * d.x)");
    StringReplace(G, "y", "(E.z + L * d.z)");
    StringReplace(G, "E", "eye");
    G += " - eye.y - L * d.y";
    StringReplace(G, "eee", "exp");

    string newFx(mFx);
    StringReplace(newFx, "exp", "eee");
    StringReplace(newFx, "x", "p.x");
    StringReplace(newFx, "y", "p.y");
    StringReplace(newFx, "eee", "exp");

    string newFy(mFy);
    StringReplace(newFy, "exp", "eee");
    StringReplace(newFy, "x", "p.x");
    StringReplace(newFy, "y", "p.y");
    StringReplace(newFy, "eee", "exp");

    string newSource(mFragSource);
    newSource.replace(newSource.find("#Fx"), 3, newFx);
    newSource.replace(newSource.find("#Fy"), 3, newFy);
    newSource.replace(newSource.find("#G"), 2, G);

    mProgram.Init(&mVertexShader, nullptr, &FragmentShader(newSource, mFileName.c_str()), mFileName.c_str());

    AABB expanded(mBox);
    const float expansion = 0.1f * (mBox.mMaxv.y - mBox.mMinv.y);
    expanded.mMinv.y -= expansion;
    expanded.mMaxv.y += expansion;

    mProgram.Use();
    mProgram.SetUniform("minv", expanded.mMinv);
    mProgram.SetUniform("maxv", expanded.mMaxv);

    mProgram.SetUniform("Lstep", UserSettings::Get().Lstep.mValue);
    mProgram.SetUniform("tolerance", UserSettings::Get().tolerance.mValue);
    mProgram.SetUniform("AAlevel", UserSettings::Get().AAlevel.mValue);
    const int w = UserSettings::Get().screenWidth;
    const int h = UserSettings::Get().screenHeight;

    const vec2 invScreenSize = 1.0f / vec2(float(w), float(h));

    mProgram.SetUniform("invScreenSize", invScreenSize);

    mProgram.SetUniform("ambient", vec3(0.3f, 0.1f, 0.1f));
    mProgram.SetUniform("diffuse", vec3(0.8f, 0.1f, 0.1f));
    mProgram.SetUniform("specular", vec3(1.0f, 1.0f, 1.0f));
    mProgram.SetUniform("shininess", 64.0f);
    mProgram.SetUniform("lightDirection", normalize(vec3(1.0f, 1.0f, 1.0f)));
    mProgram.SetUniform("lightIntensity", vec3(0.9f));
    mProgram.SetUniform("missColor", vec3(0.3f, 0.3f, 0.3f));
}

void FunctionTracer::Draw(const Camera &cam)
{
    mProgram.Use();

    const float tanfov = tanf(cam.mFoV * engine::PI / 360.0f);

    mProgram.SetUniform("eye", cam.mPosition);
    mProgram.SetUniform("U", cam.GetRight() * tanfov * cam.mAspectRatio);
    mProgram.SetUniform("W", cam.GetDirection());
    mProgram.SetUniform("V", cam.GetUp() * tanfov);

    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}
