/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "WireCube.h"
#include <vector>

using namespace engine;
using namespace glm;
using namespace std;

WireCube::WireCube(float cubeSize)
    : mProgram("Shaders/WireBox")
{
    mBoundingBox = AABB(vec3(-cubeSize), vec3(cubeSize));
    //Load mesh to GPU
    glGenBuffers(1, &mVBO);

    static const vec3 vertices[] =  
    {
        vec3(1.0f, 1.0f, 1.0f),
        vec3(1.0f, 1.0f, -1.0f),
        vec3(1.0f, -1.0f, 1.0f),
        vec3(1.0f, -1.0f, -1.0f),
        vec3(-1.0f, 1.0f, 1.0f),
        vec3(-1.0f, 1.0f, -1.0f),
        vec3(-1.0f, -1.0f, 1.0f),
        vec3(-1.0f, -1.0f, -1.0f),
    };

    static const GLushort indices[] =
    {
        0, 1, 0, 2, 0, 4,
        3, 1, 3, 2, 3, 7,
        5, 1, 5, 7, 5, 4,
        6, 2, 6, 7, 6, 4
    };

    vector<vec3> vertexArray;
    for(GLushort i : indices)
        vertexArray.push_back(vertices[i] * cubeSize);

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
}

WireCube::~WireCube(void)
{
    glDeleteBuffers(1, &mVBO);
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

