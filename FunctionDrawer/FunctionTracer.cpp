/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "FunctionTracer.h"
#include "UserSettings.h"
#include <Engine/Common/ErrorCheck.h>
#include <Engine/Common/MathFunctions.h>

using namespace std;
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
    p.Init(&vs, nullptr, &FragmentShader("Shaders/tracing"), "Shaders/tracing");
}

void FunctionTracer::Draw(const Camera &cam)
{
    p.Use();

    const float tanfov = tanf(cam.FoV * engine::PI / 360.0f) * 0.5f;

    p.SetUniform("eye", cam.position);
    p.SetUniform("U", cam.GetRight() * tanfov * cam.aspectRatio);
    p.SetUniform("W", cam.GetDirection());
    p.SetUniform("V", cam.GetUp() * tanfov);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}
