/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "FunctionDrawer.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <Engine/Core/EventHandler.h>
#include <Engine/Common/ErrorCheck.h>

using namespace std;
using namespace glm;
using namespace engine;

FunctionDrawer::FunctionDrawer(void)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &PVBO);
    glGenBuffers(1, &NVBO);
    glGenBuffers(1, &IBO);

    glBindVertexArray(VAO);
    {
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, PVBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, NVBO);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    }
    glBindVertexArray(0);
}

void FunctionDrawer::CleanUp()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &PVBO);
    glDeleteBuffers(1, &NVBO);
    glDeleteBuffers(1, &IBO);
}

void FunctionDrawer::SetFunction(const string &expressionString)
{
    F.SetFunction(expressionString);
}

void FunctionDrawer::SetXDerivative(const std::string &expressionString)
{
    Fx.SetFunction(expressionString);
}

void FunctionDrawer::SetYDerivative(const std::string &expressionString)
{
    Fy.SetFunction(expressionString);
}

void FunctionDrawer::GenerateMesh(int ctVertices)
{
    const float scale = 20.0f / float(ctVertices - 1); //TODO: setting

    vector<vec3> positions;
    positions.reserve(ctVertices * ctVertices);
    vector<vec3> normals;
    normals.reserve(ctVertices * ctVertices);

    minH = FLT_MAX;
    maxH = -FLT_MAX;
    for(GLuint i = 0; i < ctVertices; ++i)
        for(GLuint j = 0; j < ctVertices; ++j)
        {
            const vec2 xz(float(i) * scale, float(j) * scale);
            const float y = F(xz);

            maxH = std::max(maxH, y);
            minH = std::min(minH, y);

            positions.push_back(vec3(xz.x, y, xz.y));
            normals.push_back(normalize(vec3(Fx(xz), Fy(xz), -1.0f)));
        }

    glBindVertexArray(VAO);
    {
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, PVBO);
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(positions[0]), positions.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, NVBO);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), normals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    }
    glBindVertexArray(0);
}

void FunctionDrawer::GenerateIndices(int ctVertices)
{
    vector<GLuint> ind;
    ind.reserve((ctVertices - 1) * (ctVertices - 1) * 6);

    for(GLuint i = 0; i < ctVertices - 1; ++i)
    {
        for(GLuint j = 0; j < ctVertices - 1; ++j)
        {
            const GLuint top_left = i + ctVertices * j;
            const GLuint top_right = top_left + 1;
            const GLuint bottom_left = top_left + ctVertices;
            const GLuint bottom_right = bottom_left + 1;

            ind.push_back(top_left);
            ind.push_back(top_right);
            ind.push_back(bottom_left);

            ind.push_back(top_right);
            ind.push_back(bottom_right);
            ind.push_back(bottom_left);
        }
    }

    indices.SetData(ind);
    glBindVertexArray(VAO);
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.GetSizeInBytes(), indices.GetData(), GL_STATIC_DRAW);
    }
    glBindVertexArray(0);
}

void FunctionDrawer::Draw() const
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.Size(), indices.GetIndexDataType(), nullptr);
    glBindVertexArray(0);
}
