/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef FD_FUNCTIONTRACER_H
#define FD_FUNCTIONTRACER_H

#include <string>
#include <glm/glm.hpp>
#include <Engine/GL/Program.h>
#include <Engine/Core/Camera.h>

class FunctionTracer
{
public:
    FunctionTracer(void);
    void CleanUp();

    void SetFunction(const std::string &expressionString);
    void SetXDerivative(const std::string &expressionString);
    void SetYDerivative(const std::string &expressionString);
    void ApplyFunction();

    void Draw(const engine::Camera &cam);

private:
    engine::Program p;

    std::string F;
    std::string Fx;
    std::string Fy;

    GLuint VAO;
    GLuint VBO;
};

#endif
