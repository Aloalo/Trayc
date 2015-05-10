/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef FD_FUNCTIONDRAWER_H
#define FD_FUNCTIONDRAWER_H

#include <string>
#include <glm/glm.hpp>
#include <Engine/GL/Program.h>
#include <Engine/Core/Camera.h>
#include <Engine/Geometry/IndexContainer.h>
#include "TwoVariableFunction.h"

struct Batch
{
    GLuint VAO;
    GLuint VBO;
    GLsizei count;
};


class FunctionRasterizer
{
public:
    FunctionRasterizer(void);
    void CleanUp();

    void SetFunction(const std::string &expressionString);
    void SetXDerivative(const std::string &expressionString);
    void SetYDerivative(const std::string &expressionString);
    void ApplyFunction();

    void Draw(const engine::Camera &cam);

private:
    void GenerateMesh(int ctVertices);
    void GenerateIndices(int ctVertices);

    engine::Program p;

    TwoVariableFunction F;
    TwoVariableFunction Fx;
    TwoVariableFunction Fy;

    std::vector<Batch> batches;
    GLuint IBO;
    GLsizei ctIndices;
};

#endif
