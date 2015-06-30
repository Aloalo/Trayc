/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "FunctionTracer.h"
#include "UserSettings.h"
#include <Engine/Utils/Utilities.h>
#include <Engine/Utils/MathFunctions.h>
#include <iostream>
#include "TmpHandlers.h"

using namespace std;
using namespace glm;
using namespace engine;

FunctionTracer::FunctionTracer(void)
    : FunctionDrawer("Shaders/tracing")
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    static const GLfloat quad[] = 
    { 
        -1.0f, -1.0f,
        1.0f, -1.0f,
        1.0f, 1.0f,
        -1.0f, 1.0f
    };

    glBindVertexArray(VAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    }
    glBindVertexArray(0);

}

FunctionTracer::~FunctionTracer(void)
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void FunctionTracer::SetFunction(const std::string &F, const std::string &Fx, const std::string &Fy)
{
    FunctionDrawer::SetFunction(F, Fx, Fy);
}

void FunctionTracer::ApplyFunction()
{
    string G(F);
    StringReplace(G, "exp", "eee");
    StringReplace(G, "x", "(E.x + L * d.x)");
    StringReplace(G, "y", "(E.z + L * d.z)");
    StringReplace(G, "E", "eye");
    G += " - eye.y - L * d.y";
    StringReplace(G, "eee", "exp");

    string newFx(Fx);
    StringReplace(newFx, "exp", "eee");
    StringReplace(newFx, "x", "p.x");
    StringReplace(newFx, "y", "p.y");
    StringReplace(newFx, "eee", "exp");

    string newFy(Fy);
    StringReplace(newFy, "exp", "eee");
    StringReplace(newFy, "x", "p.x");
    StringReplace(newFy, "y", "p.y");
    StringReplace(newFy, "eee", "exp");



    string newSource(fragSource);
    newSource.replace(newSource.find("#Fx"), 3, newFx);
    newSource.replace(newSource.find("#Fy"), 3, newFy);
    newSource.replace(newSource.find("#G"), 2, G);

    p.Init(&vs, nullptr, &FragmentShader(newSource, fileName.c_str()), fileName.c_str());

    AABB expanded(box);
    const float expansion = 0.1f * (box.mMaxv.y - box.mMinv.y);
    expanded.mMinv.y -= expansion;
    expanded.mMaxv.y += expansion;

    p.Use();
    p.SetUniform("minv", expanded.mMinv);
    p.SetUniform("maxv", expanded.mMaxv);

    p.SetUniform("Lstep", UserSettings::Get().Lstep.mValue);
    p.SetUniform("tolerance", UserSettings::Get().tolerance.mValue);
    p.SetUniform("AAlevel", UserSettings::Get().AAlevel.mValue);
    int w, h;
    sdlHandler.GetWindowSize(w, h);
    const vec2 invScreenSize = 1.0f / vec2(float(w), float(h));

    p.SetUniform("invScreenSize", invScreenSize);

    p.SetUniform("ambient", vec3(0.3f, 0.1f, 0.1f));
    p.SetUniform("diffuse", vec3(0.8f, 0.1f, 0.1f));
    p.SetUniform("specular", vec3(1.0f, 1.0f, 1.0f));
    p.SetUniform("shininess", 64.0f);
    p.SetUniform("lightDirection", normalize(vec3(1.0f, 1.0f, 1.0f)));
    p.SetUniform("lightIntensity", vec3(0.9f));
    p.SetUniform("missColor", vec3(0.3f, 0.3f, 0.3f));
}

void FunctionTracer::Draw(const Camera &cam)
{
    p.Use();

    const float tanfov = tanf(cam.mFoV * engine::PI / 360.0f);

    p.SetUniform("eye", cam.mPosition);
    p.SetUniform("U", cam.GetRight() * tanfov * cam.mAspectRatio);
    p.SetUniform("W", cam.GetDirection());
    p.SetUniform("V", cam.GetUp() * tanfov);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}
