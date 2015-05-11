/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "FunctionTracer.h"
#include "UserSettings.h"
#include <Engine/Common/Utilities.h>
#include <Engine/Common/MathFunctions.h>
#include <iostream>

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
    string newF(F);
    StringReplace(newF, "x", "p.x");
    StringReplace(newF, "y", "p.y");
    string newFx(Fx);
    StringReplace(newFx, "x", "p.x");
    StringReplace(newFx, "y", "p.y");
    string newFy(Fy);
    StringReplace(newFy, "x", "p.x");
    StringReplace(newFy, "y", "p.y");


    string newSource(fragSource);
    newSource.replace(newSource.find("#Fx"), 3, newFx);
    newSource.replace(newSource.find("#Fy"), 3, newFy);
    newSource.replace(newSource.find("#F"), 2, newF);

    p.Init(&vs, nullptr, &FragmentShader(newSource, fileName.c_str()), fileName.c_str());

    p.Use();
    p.SetUniform("Xmin", UserSettings::Get().minX.x);
    p.SetUniform("Xmax", UserSettings::Get().maxX.x);
    p.SetUniform("Zmin", UserSettings::Get().minY.x);
    p.SetUniform("Zmax", UserSettings::Get().maxY.x);
    p.SetUniform("drawDistance", UserSettings::Get().drawDistance.x);
    p.SetUniform("Lstep", UserSettings::Get().Lstep.x);

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

    const float tanfov = tanf(cam.FoV * engine::PI / 360.0f);

    p.SetUniform("eye", cam.position);
    p.SetUniform("U", cam.GetRight() * tanfov * cam.aspectRatio);
    p.SetUniform("W", cam.GetDirection());
    p.SetUniform("V", cam.GetUp() * tanfov);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}
