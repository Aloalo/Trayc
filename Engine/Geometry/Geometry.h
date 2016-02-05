/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_GEOMETRY_H
#define EN_GEOMETRY_H

#include <Engine/Utils/StlExtensions.hpp>
#include <vector>
#include <map>

namespace engine
{
    template<class T>
    std::vector<glm::vec3> CalculateNormals(const std::vector<T> &positions, GLenum orientation);

    template<class T>
    void InterpolateNormals(const std::vector<T> &positions, std::vector<glm::vec3> &normals);
    
    template<class T>
    static inline bool GetSimilarVertexIndex(const T &packed, std::map<T, GLuint> &vertexToOutIndex, unsigned int &result);

    template<class T>
    void IndexMesh(std::vector<T> &vertices, std::vector<GLuint> &outIndexData);

    //IMPLEMENTATION//IMPLEMENTATION//IMPLEMENTATION//IMPLEMENTATION//IMPLEMENTATION//IMPLEMENTATION//IMPLEMENTATION//IMPLEMENTATION//IMPLEMENTATION//IMPLEMENTATION//
    
    template<class T>
    std::vector<glm::vec3> CalculateNormals(const std::vector<T> &positions, GLenum orientation)
    {
        const int ctVertices = positions.size();
        const GLfloat flip = orientation == GL_CCW ? 1.0f : -1.0f;

        std::vector<glm::vec3> normals;
        normals.reserve(ctVertices);
        for(int i = 0; i < ctVertices; i += 3)
        {
            glm::vec3 normal = normalize(glm::cross(glm::vec3(positions[i + 1] - positions[i]), glm::vec3(positions[i + 2] - positions[i])));

            normals.push_back(flip * normal);
            normals.push_back(flip * normal);
            normals.push_back(flip * normal);
        }

        return normals;
    }

    template<class T>
    void InterpolateNormals(const std::vector<T> &positions, std::vector<glm::vec3> &normals)
    {
        using pvi = std::pair<T, int>;

        const int ctVertices = positions.size();
        std::vector<pvi> out(ctVertices);

        for(int i = 0; i < ctVertices; i++)
            out[i] = pvi(positions[i], i);

        stdext::sort(out, [](const pvi &a, const pvi &b)
        {
            return memcmp((const void*)&a, (const void*)&b, sizeof(pvi)) < 0;
        });

        int index = 0;
        glm::vec3 interpNor = normals[out[0].second];
        for(int i = 1; i < ctVertices; i++)
        {
            while(i < ctVertices && out[index].first == out[i].first)
            {
                interpNor += normals[out[i].second];
                i++;
            }
            interpNor = glm::normalize(interpNor);

            for(int j = index; j < i; j++)
                normals[out[j].second] = interpNor;

            interpNor = vec3(0.0f);
            index = i;
        }
    }
    
    template<class T>
    static inline bool GetSimilarVertexIndex(const T &packed, const std::map<T, GLuint> &vertexToOutIndex, unsigned int &result)
    {
        const auto it = vertexToOutIndex.find(packed);
        if (it == vertexToOutIndex.end())
            return false;
        else
        {
            result = it->second;
            return true;
        }
    }

    template<class T>
    std::vector<GLuint> IndexMesh(std::vector<T> &vertices)
    {
        const GLuint ctVertices = vertices.size();

        std::vector<T> copyVertices(vertices);
        vertices.clear();
        std::vector<GLuint> outIndexData;
        outIndexData.reserve(ctVertices);

        std::map<T, GLuint> vertexToOutIndex;

        for(GLuint i = 0; i < ctVertices; ++i)
        {
            GLuint index;
            bool found = GetSimilarVertexIndex<T>(copyVertices[i], vertexToOutIndex, index);

            if(found)
                outIndexData.push_back(index);
            else
            {
                vertices.push_back(copyVertices[i]);
                const GLuint ctOutVertices = vertices.size();
                outIndexData.push_back(ctOutVertices - 1);
                vertexToOutIndex[copyVertices[i]] = ctOutVertices - 1;
            }
        }
        return outIndexData;
     }
}
#endif
