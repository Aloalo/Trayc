/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "FunctionRasterizer.h"
#include "UserSettings.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <Engine/Core/EventHandler.h>
#include <Engine/Common/ErrorCheck.h>

#include <half/include/half.hpp>


using namespace std;
using namespace glm;
using namespace engine;
using namespace half_float;

struct PackedNormal
{
    PackedNormal(int x, int y, int z)
        : x(x), y(y), z(z), a(0)
    {
    }

    int x:10; 
    int y:10;
    int z:10; 
    int a:2; 
};

//indices per batch
static const GLuint ctMaxBatchIndices = (1 << 16)  - 1;

FunctionRasterizer::FunctionRasterizer(void)
    : p("Shaders/Simple")
{
    p.SetUniform("ambient", vec3(0.3f, 0.1f, 0.1f));
    p.SetUniform("diffuse", vec3(0.8f, 0.1f, 0.1f));
    p.SetUniform("specular", vec3(1.0f, 1.0f, 1.0f));
    p.SetUniform("shininess", 64.0f);
    p.SetUniform("lightDirection", normalize(vec3(1.0f, 1.0f, 1.0f)));
    p.SetUniform("lightIntensity", vec3(0.9f));

    batches.resize(0);
    glGenBuffers(1, &IBO);
}

void FunctionRasterizer::CleanUp()
{
    for(auto &batch : batches)
    {
        glDeleteVertexArrays(1, &batch.VAO);
        glDeleteBuffers(1, &batch.VBO);
    }
    glDeleteBuffers(1, &IBO);
    p.Delete();
}

void FunctionRasterizer::SetFunction(const string &expressionString)
{
    F.SetFunction(expressionString);
}

void FunctionRasterizer::SetXDerivative(const std::string &expressionString)
{
    Fx.SetFunction(expressionString);
}

void FunctionRasterizer::SetYDerivative(const std::string &expressionString)
{
    Fy.SetFunction(expressionString);
}

void FunctionRasterizer::GenerateMesh(int ctVertices)
{
    cout << "Building mesh ... "; 

    const vec2 scale = vec2(UserSettings::Get().scaleX.x, UserSettings::Get().scaleY.x) / float(ctVertices - 1); //TODO: setting
    const vec2 offset(UserSettings::Get().offsetX.x, UserSettings::Get().offsetY.x);

    vector<unsigned char> vertices;

    int normalSize, positionSize;
    GLenum positionType, normalType;
    GLboolean normalNormalize;
    GLint normalNumElems;
    if(UserSettings::Get().smallData)
    {
        positionSize = 3 * sizeof(half);
        normalSize = sizeof(int);
        positionType = GL_HALF_FLOAT;
        normalType = GL_INT_2_10_10_10_REV;
        normalNormalize = GL_TRUE;
        normalNumElems = 4;
    }
    else
    {
        positionSize = sizeof(vec3);
        normalSize = sizeof(vec3);
        positionType = GL_FLOAT;
        normalType = GL_FLOAT;
        normalNormalize = GL_FALSE;
        normalNumElems = 3;
    }
    const int vertexSize = normalSize + positionSize;

    try
    {
        vertices.reserve(ctVertices * ctVertices * vertexSize);
    }
    catch(exception &e)
    {
        cerr << "Caught exception: " << e.what() << endl;
    }
    
    for(GLuint i = 0; i < ctVertices; ++i)
    {
        for(GLuint j = 0; j < ctVertices; ++j)
        {
            const vec2 xz = vec2(float(i) , float(j)) * scale + offset;
            const float y = F(xz);

            const vec3 normal(normalize(-vec3(Fx(xz), -1.0f, Fy(xz))));
            if(UserSettings::Get().smallData)
            {
                const half position[3] = {half(xz.x), half(y), half(xz.y)};

                vertices.insert(vertices.end(), vertexSize, 0);
                const unsigned char *start = &vertices[vertices.size() - vertexSize];
                memcpy((void*)start, (void*)position, positionSize);

                const PackedNormal pNormal((int)(normal.x * 511.0f), (int)(normal.y * 511.0f), (int)(normal.z * 511.0f));

                memcpy((void*)(start + positionSize), (void*)&pNormal, normalSize);
            }
            else
            {
                const float position[3] = {xz.x, y, xz.y};

                vertices.insert(vertices.end(), vertexSize, 0);
                const unsigned char *start = &vertices[vertices.size() - vertexSize];
                memcpy((void*)start, (void*)position, positionSize);
                memcpy((void*)(start + positionSize), (void*)&normal, normalSize);
            }
        }
    }

    for(Batch &batch : batches)
    {
        glDeleteVertexArrays(1, &batch.VAO);
        glDeleteBuffers(1, &batch.VBO);
    }

    const int ctStripesPerBatch = ctMaxBatchIndices / ctVertices;
    const int ctVerticesPerBatch = ctStripesPerBatch * ctVertices;
    const int ctBatches = (ctVertices * ctVertices) / ctVerticesPerBatch;
    batches.resize(ctBatches + 1);

    for(int i = 0; i <= ctBatches; ++i)
    {
        const int stripeOffset = i * ctVertices * vertexSize;

        glGenVertexArrays(1, &batches[i].VAO);
        glGenBuffers(1, &batches[i].VBO);

        int ctVert;
        if(i != ctBatches)
        {
            batches[i].count = ctIndices;
            ctVert = ctVerticesPerBatch;
        }
        else
        {
            const int ctRemainingStripes = ctVertices - ctStripesPerBatch * ctBatches + ctBatches;
            batches[i].count = ctIndices;//(ctRemainingStripes - 1) * (ctVertices - 1) * 6;
            ctVert = (ctVertices * ctVertices) % ctVerticesPerBatch + stripeOffset / vertexSize;
        }

        glBindVertexArray(batches[i].VAO);
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

            glBindBuffer(GL_ARRAY_BUFFER, batches[i].VBO);
            glBufferData(GL_ARRAY_BUFFER, ctVert * vertexSize, (void*)(vertices.data()+(i*ctVerticesPerBatch*vertexSize - stripeOffset)), GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, positionType, GL_FALSE, vertexSize, (void*)0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, normalNumElems, normalType, normalNormalize, vertexSize, (void*)positionSize);
        }
        glBindVertexArray(0);
    }

    cout << "DONE" << endl; 
}

void FunctionRasterizer::GenerateIndices(int ctVertices)
{
    assert(2 * ctVertices < ctMaxBatchIndices);

    const int ctStripes = ctMaxBatchIndices / ctVertices;
    const int ctBatchIndices = ctStripes * ctVertices;

    vector<int> intIndices;
    intIndices.reserve((ctStripes - 1) * (ctVertices - 1) * 6);

    for(int i = 0; i < ctStripes - 1; ++i)
    {
        for(int j = 0; j < ctVertices - 1; ++j)
        {
            const int top_left = i * ctVertices + j;
            const int top_right = top_left + 1;
            const int bottom_left = top_left + ctVertices;
            const int bottom_right = bottom_left + 1;

            intIndices.push_back(top_left);
            intIndices.push_back(top_right);
            intIndices.push_back(bottom_left);

            intIndices.push_back(top_right);
            intIndices.push_back(bottom_right);
            intIndices.push_back(bottom_left);
        }
    }

    const vector<GLushort> indices(intIndices.begin(), intIndices.end());
    ctIndices = indices.size();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void FunctionRasterizer::ApplyFunction()
{
    GenerateIndices(UserSettings::Get().ctVertices);
    GenerateMesh(UserSettings::Get().ctVertices);
}

void FunctionRasterizer::Draw(const Camera &cam)
{
    p.Use();

    const mat4 V = cam.GetViewMatrix();
    p.SetUniform("MVP", cam.GetProjectionMatrix() * V);
    p.SetUniform("invVxeye", inverse(V) * vec4(0.0f, 0.0f, 0.0f, 1.0f));


    for(const Batch &batch : batches)
    {
        glBindVertexArray(batch.VAO);
        glDrawElements(GL_TRIANGLES, batch.count, GL_UNSIGNED_SHORT, nullptr);
        GLErrCheck(true);
        glBindVertexArray(0);
    }
}
