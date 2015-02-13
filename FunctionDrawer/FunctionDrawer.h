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

    float minH, maxH;

private:
    TwoVariableFunction F;
    TwoVariableFunction Fx;
    TwoVariableFunction Fy;

    //Mesh stuff
    engine::IndexContainer indices;

    //GL stuff
    GLuint VAO;
    GLuint VBO;
    GLuint IBO;
};

#endif
