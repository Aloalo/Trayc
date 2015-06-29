/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "FunctionRasterizer.h"
#include "UserSettings.h"
#include <Engine/Utils/ErrorCheck.h>
#include <Engine/Utils/Utilities.h>

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
    : FunctionDrawer("Shaders/Simple")
{
    batches.resize(0);
    glGenBuffers(1, &IBO);
}

FunctionRasterizer::~FunctionRasterizer(void)
{
    for(auto &batch : batches)
    {
        glDeleteVertexArrays(1, &batch.VAO);
        glDeleteBuffers(1, &batch.VBO);
    }
    glDeleteBuffers(1, &IBO);
}

void FunctionRasterizer::SetFunction(const string &F, const string &Fx, const string &Fy)
{
    FunctionDrawer::SetFunction(F, Fx, Fy);
    func.SetFunction(F);
}

int FunctionRasterizer::GenerateMesh(int ctVertices)
{
    assert(ctVertices > 0);

    box = AABB();

    cout << "Building mesh ... "; 

    const vec2 minv(UserSettings::Get().minX.x, UserSettings::Get().minY.x);
    const vec2 maxv(UserSettings::Get().maxX.x, UserSettings::Get().maxY.x);

    const vec2 scale = (maxv - minv) / float(ctVertices - 1);

    vector<half> vertices;
    const GLsizei vertexSize = 3 * sizeof(half);
    int memoryUsed = 0;

    try
    {
        vertices.reserve(ctVertices * ctVertices * 3);
    }
    catch(exception &e)
    {
        cerr << "Caught exception: " << e.what() << endl;
    }
    
    for(GLuint i = 0; i < ctVertices; ++i)
    {
        for(GLuint j = 0; j < ctVertices; ++j)
        {
            const vec2 xz = vec2(float(i) , float(j)) * scale + minv;
            const vec3 p(xz.x, func(xz), xz.y);

            box |= p;

            vertices.push_back(half(p.x));
            vertices.push_back(half(p.y));
            vertices.push_back(half(p.z));
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
            batches[i].count = (ctRemainingStripes - 1) * (ctVertices - 1) * 6;
            ctVert = (ctVertices * ctVertices) % ctVerticesPerBatch + stripeOffset / vertexSize;
        }

        glBindVertexArray(batches[i].VAO);
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

            glBindBuffer(GL_ARRAY_BUFFER, batches[i].VBO);
            glBufferData(GL_ARRAY_BUFFER, ctVert * vertexSize, (void*)(vertices.data()+(i*ctVerticesPerBatch*vertexSize - stripeOffset) / 2), GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_HALF_FLOAT, GL_FALSE, 0, (void*)0);
        }
        glBindVertexArray(0);
        memoryUsed += (ctVert * vertexSize);
    }

    cout << "DONE" << endl;
    return memoryUsed;
}

int FunctionRasterizer::GenerateIndices(int ctVertices)
{
    assert(2 * ctVertices < ctMaxBatchIndices);

    const int ctStripes = ctMaxBatchIndices / ctVertices;

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

    return indices.size() * sizeof(indices[0]);
}

void FunctionRasterizer::ApplyFunction()
{
    const int memIndices = GenerateIndices(UserSettings::Get().ctVertices);
    const int memVertices = GenerateMesh(UserSettings::Get().ctVertices);
    const int total = memIndices + memVertices;

    cout << "  Memory used indices: " << memIndices << endl;
    cout << "  Memory used vertices: " << memVertices << endl;
    cout << "  Total: " << total << " (" << (total >> 20) <<  "MB)" << endl << endl;

    string newFx(Fx);
    //handle exp
    StringReplace(newFx, "exp", "eee");
    StringReplace(newFx, "x", "p.x");
    StringReplace(newFx, "y", "p.y");
    StringReplace(newFx, "eee", "exp");

    string newFy(Fy);
    StringReplace(newFy, "exp", "eee");
    StringReplace(newFy, "x", "p.x");
    StringReplace(newFy, "y", "p.y");
    StringReplace(newFy, "eee", "exp");


    string newSource(fragSource);
    newSource.replace(newSource.find("#Fx"), 3, newFx);
    newSource.replace(newSource.find("#Fy"), 3, newFy);

    p.Init(&vs, nullptr, &FragmentShader(newSource, fileName.c_str()), fileName.c_str());

    p.Use();
    p.SetUniform("ambient", vec3(0.3f, 0.1f, 0.1f));
    p.SetUniform("diffuse", vec3(0.8f, 0.1f, 0.1f));
    p.SetUniform("specular", vec3(1.0f, 1.0f, 1.0f));
    p.SetUniform("shininess", 64.0f);
    p.SetUniform("lightDirection", normalize(vec3(1.0f, 1.0f, 1.0f)));
    p.SetUniform("lightIntensity", vec3(0.9f));
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
        glBindVertexArray(0);
    }
}
