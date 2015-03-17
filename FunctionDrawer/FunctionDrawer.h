/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef FD_FUNCTIONDRAWER_H
#define FD_FUNCTIONDRAWER_H

#include <string>
#include <glm/glm.hpp>
#include <Engine/GL/Program.h>
#include <Engine/Geometry/IndexContainer.h>
#include "TwoVariableFunction.h"

struct Batch
{
    GLuint VAO;
    GLuint VBO;
    GLsizei count;
};


class FunctionDrawer
{
public:
    FunctionDrawer(void);
    void CleanUp();

    void GenerateMesh(int ctVertices);
    void GenerateIndices(int ctVertices);

    void SetFunction(const std::string &expressionString);
    void SetXDerivative(const std::string &expressionString);
    void SetYDerivative(const std::string &expressionString);

    void Draw() const;

private:
    TwoVariableFunction F;
    TwoVariableFunction Fx;
    TwoVariableFunction Fy;

    std::vector<Batch> batches;
    GLuint IBO;
    GLuint leftoverIBO;
    GLsizei ctIndices;
};

#endif
