/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "FunctionRasterizer.h"
#include "UserSettings.h"
#include <Engine/Utils/ErrorCheck.h>
#include <Engine/Utils/Utilities.h>

#include "TwoVariableFunction.h"
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
    mBatches.resize(0);
    glGenBuffers(1, &mIBO);
}

FunctionRasterizer::~FunctionRasterizer(void)
{
    for(auto &batch : mBatches)
    {
        glDeleteVertexArrays(1, &batch.VAO);
        glDeleteBuffers(1, &batch.VBO);
    }
    glDeleteBuffers(1, &mIBO);
}

void FunctionRasterizer::SetFunction(const string &F, const string &Fx, const string &Fy)
{
    FunctionDrawer::SetFunction(F, Fx, Fy);
}

int FunctionRasterizer::GenerateMesh(int ctVertices)
{
    assert(ctVertices > 0);

    cout << "Building mesh ... "; 

    const vec2 minv(UserSettings::Get().minX.mValue, UserSettings::Get().minY.mValue);
    const vec2 maxv(UserSettings::Get().maxX.mValue, UserSettings::Get().maxY.mValue);
    const vec2 domainSize(maxv - minv);
    const vec2 scale(domainSize / float(ctVertices - 1));

    vector<half> vertices;

    try
    {
        vertices.reserve(ctVertices * ctVertices * 3);
    }
    catch(exception &e)
    {
        cerr << "Caught exception: " << e.what() << endl;
    }
    
    TwoVariableFunction f(mF);
    AABB box;
    const GLuint uiCtVertices = static_cast<GLuint>(ctVertices);
    for(GLuint i = 0; i < uiCtVertices; ++i)
    {
        const float x = float(i) * scale.x + minv.x;
        const half xhf(x);
        for(GLuint j = 0; j < uiCtVertices; ++j)
        {
            const float z = float(j) * scale.y + minv.y;

            const vec3 p(x, f(x, z), z);

            box |= p;

            vertices.push_back(xhf);
            vertices.push_back(half(p.y));
            vertices.push_back(half(p.z));
        }
    }
    mBox = box;

    for(Batch &batch : mBatches)
    {
        glDeleteVertexArrays(1, &batch.VAO);
        glDeleteBuffers(1, &batch.VBO);
    }

    const int ctStripesPerBatch = ctMaxBatchIndices / ctVertices;
    const int ctVerticesPerBatch = ctStripesPerBatch * ctVertices;
    const int ctBatches = (ctVertices * ctVertices) / ctVerticesPerBatch;
    const GLsizei vertexSize = 3 * sizeof(half);
    mBatches.resize(ctBatches + 1);

    int memoryUsed = 0;
    for(int i = 0; i <= ctBatches; ++i)
    {
        const int stripeOffset = i * ctVertices * vertexSize;

        glGenVertexArrays(1, &mBatches[i].VAO);
        glGenBuffers(1, &mBatches[i].VBO);

        int ctVert;
        if(i != ctBatches)
        {
            mBatches[i].count = mCtIndices;
            ctVert = ctVerticesPerBatch;
        }
        else
        {
            const int ctRemainingStripes = ctVertices - ctStripesPerBatch * ctBatches + ctBatches;
            mBatches[i].count = (ctRemainingStripes - 1) * (ctVertices - 1) * 6;
            ctVert = (ctVertices * ctVertices) % ctVerticesPerBatch + stripeOffset / vertexSize;
        }

        glBindVertexArray(mBatches[i].VAO);
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);

            glBindBuffer(GL_ARRAY_BUFFER, mBatches[i].VBO);
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
    mCtIndices = indices.size();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
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

    string newFx(mFx);
    //handle exp
    StringReplace(newFx, "exp", "eee");
    StringReplace(newFx, "x", "p.x");
    StringReplace(newFx, "y", "p.y");
    StringReplace(newFx, "eee", "exp");

    string newFy(mFy);
    StringReplace(newFy, "exp", "eee");
    StringReplace(newFy, "x", "p.x");
    StringReplace(newFy, "y", "p.y");
    StringReplace(newFy, "eee", "exp");


    string newSource(mFragSource);
    newSource.replace(newSource.find("#Fx"), 3, newFx);
    newSource.replace(newSource.find("#Fy"), 3, newFy);

    mProgram.Init(&mVertexShader, nullptr, &FragmentShader(newSource, mFileName.c_str()), mFileName.c_str());

    mProgram.Use();
    mProgram.SetUniform("ambient", vec3(0.3f, 0.1f, 0.1f));
    mProgram.SetUniform("diffuse", vec3(0.8f, 0.1f, 0.1f));
    mProgram.SetUniform("specular", vec3(1.0f, 1.0f, 1.0f));
    mProgram.SetUniform("shininess", 64.0f);
    mProgram.SetUniform("lightDirection", normalize(vec3(1.0f, 1.0f, 1.0f)));
    mProgram.SetUniform("lightIntensity", vec3(0.9f));
}

void FunctionRasterizer::Draw(const Camera &cam)
{
    mProgram.Use();

    const mat4 V = cam.GetViewMatrix();
    mProgram.SetUniform("MVP", cam.GetProjectionMatrix() * V);
    mProgram.SetUniform("invVxeye", inverse(V) * vec4(0.0f, 0.0f, 0.0f, 1.0f));


    for(const Batch &batch : mBatches)
    {
        glBindVertexArray(batch.VAO);
        glDrawElements(GL_TRIANGLES, batch.count, GL_UNSIGNED_SHORT, nullptr);
        glBindVertexArray(0);
    }
}
