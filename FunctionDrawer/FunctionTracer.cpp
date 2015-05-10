/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "FunctionTracer.h"
#include "UserSettings.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <Engine/Common/ErrorCheck.h>
#include <Engine/Common/MathFunctions.h>

using namespace std;
using namespace glm;
using namespace engine;

//indices per batch
static const GLuint ctMaxBatchIndices = (1 << 16)  - 1;

FunctionTracer::FunctionTracer(void)
    : p("Shaders/tracing")
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

void FunctionTracer::CleanUp()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    p.Delete();
}

void FunctionTracer::SetFunction(const string &expressionString)
{
    F = expressionString;
}

void FunctionTracer::SetXDerivative(const std::string &expressionString)
{
    Fx = expressionString;
}

void FunctionTracer::SetYDerivative(const std::string &expressionString)
{
    Fy = expressionString;
}

void FunctionTracer::ApplyFunction()
{

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
