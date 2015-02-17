/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "FunctionDrawer.h"
#include "UserSettings.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <Engine/Core/EventHandler.h>
#include <Engine/Common/ErrorCheck.h>

#include <vcacheopt.h>
#include <half/include/half.hpp>


using namespace std;
using namespace glm;
using namespace engine;
using namespace half_float;

struct PackedNormal
{
    int x:10; 
    int y:10;
    int z:10; 
    int a:2; 
};

FunctionDrawer::FunctionDrawer(void)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
}

void FunctionDrawer::CleanUp()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
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
    const vec2 scale = vec2(UserSettings::Get().scaleX.x, UserSettings::Get().scaleY.x) / float(ctVertices - 1); //TODO: setting
    const vec2 offset(UserSettings::Get().offsetX.x, UserSettings::Get().offsetY.x);

    vector<unsigned char> vertices;
    vertices.reserve(ctVertices * ctVertices * 2 * sizeof(vec3));

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


    minH = FLT_MAX;
    maxH = -FLT_MAX;
    for(GLuint i = 0; i < ctVertices; ++i)
        for(GLuint j = 0; j < ctVertices; ++j)
        {
            const vec2 xz = vec2(float(i) , float(j)) * scale + offset;
            const float y = F(xz);

            maxH = std::max(maxH, y);
            minH = std::min(minH, y);

            const vec3 normal(normalize(-vec3(Fx(xz), -1.0f, Fy(xz))));
            if(UserSettings::Get().smallData)
            {
                const half position[3] = {half(xz.x), half(y), half(xz.y)};

                vertices.insert(vertices.end(), vertexSize, 0);
                const unsigned char *start = &vertices[vertices.size() - vertexSize];
                memcpy((void*)start, (void*)position, positionSize);

                PackedNormal pNormal;
                pNormal.x = (int)(normal.x * 511.0f);
                pNormal.y = (int)(normal.y * 511.0f);
                pNormal.z = (int)(normal.z * 511.0f);

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

    glBindVertexArray(VAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, positionType, GL_FALSE, vertexSize, (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, normalNumElems, normalType, normalNormalize, vertexSize, (void*)positionSize);
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


    if(UserSettings::Get().useOptimization)
    {
        cout << "Optimizing GPU cache..." << endl;
        const int tri_count = (ctVertices - 1) * (ctVertices - 1) * 2;
        VertexCache vertex_cache;
        int misses = vertex_cache.GetCacheMissCount((int*)&ind[0], tri_count);
        cout << "Cache misses before optimization: " << misses << endl;
        cout << "ACMR: " << float(misses) / float(tri_count) << endl;

        VertexCacheOptimizer vco;
        VertexCacheOptimizer::Result res = vco.Optimize((int*)&ind[0], tri_count);
        if(res)
        {
            cerr << "Cache optimization error: " << res;
        }
        else
        {
            misses = vertex_cache.GetCacheMissCount((int*)&ind[0], tri_count);
            cout << "Cache misses after optimization: " << misses << endl;
            cout << "ACMR: " << float(misses) / float(tri_count) << endl;
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
