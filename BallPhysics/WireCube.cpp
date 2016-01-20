/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "WireCube.h"
#include <vector>
#include <Engine/Geometry/GeometryMeshes.h>

using namespace engine;
using namespace glm;
using namespace std;

WireCube::WireCube(float cubeSize)
    : mProgram("Shaders/WireBox")
{
    //Load mesh to GPU
    glGenBuffers(1, &mVBO);

    vector<vec3> vertexArray(GetCubeMesh(false, true).mPositions);
    for(vec3 &p : vertexArray)
        p *= cubeSize;

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof(vec3), vertexArray.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    }
    glBindVertexArray(0);

    mCount = vertexArray.size();
    //Init the program
    mProgram.Use();
    mProgram.SetUniform("color", vec3(1.0f, 0.3f, 0.3f));
    mProgram.Unbind();

    glEnable(GL_LINE_SMOOTH);
}

WireCube::~WireCube(void)
{
    glDeleteBuffers(1, &mVBO);
    glDeleteVertexArrays(1, &mVAO);
    mProgram.Delete();
}

void WireCube::Draw(const RenderingContext &rContext) const
{
    mProgram.Use();
    mProgram.SetUniform("MVP", rContext.mVP);

    glBindVertexArray(mVAO);
    glDrawArrays(GL_LINES, 0, mCount);
    glBindVertexArray(0);
    mProgram.Unbind();
}

