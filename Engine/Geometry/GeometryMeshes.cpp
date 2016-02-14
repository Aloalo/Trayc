/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Geometry/GeometryMeshes.h>
#include <GL/glew.h>
#include <algorithm>

using namespace std;
using namespace glm;

namespace engine
{
    static const float EPS = 1e-4f;

    static inline bool GetSimilarVertexIndex(int vertIdx, const TriangleMesh &mesh, const TriangleMesh &meshCopy, const vector<GLuint> &vertexToOutIndex, GLuint &result)
    {
        const vec3 &p = meshCopy.mPositions[vertIdx];
        const vec3 &n = meshCopy.mNormals[vertIdx];

        const auto it = find_if(vertexToOutIndex.begin(), vertexToOutIndex.end(), [&](int idx)
        {
            const vec3 &p1 = mesh.mPositions[idx];
            const vec3 &n1 = mesh.mNormals[idx];

            const vec3 dp = abs(p - p1);
            const vec3 dn = abs(n - n1);

            if(dp.x < EPS && dp.y < EPS && dp.z < EPS &&
                dn.x < EPS && dn.y < EPS && dn.z < EPS)
                return true;
            return false;

        });

        if(it == vertexToOutIndex.end())
            return false;
        else
        {
            result = *it;
            return true;
        }
    }
    
    static void IndexMesh(TriangleMesh &mesh)
    {
        const GLuint ctVertices = mesh.mPositions.size();

        const TriangleMesh meshCopy(mesh);

        mesh.Clear();
        mesh.mIndices.reserve(ctVertices);

        vector<GLuint> vertexToOutIndex;

        for(GLuint i = 0; i < ctVertices; ++i)
        {
            GLuint index;
            const bool found = GetSimilarVertexIndex(i, mesh, meshCopy, vertexToOutIndex, index);

            if(found)
                mesh.mIndices.push_back(index);
            else
            {
                const GLuint ctOutVertices = mesh.mPositions.size();
                mesh.mPositions.push_back(meshCopy.mPositions[i]);
                mesh.mNormals.push_back(meshCopy.mNormals[i]);
                mesh.mIndices.push_back(ctOutVertices);
                vertexToOutIndex.push_back(ctOutVertices);
            }
        }
    }

    template<typename T, int sz>
    static inline int size(T(&)[sz])
    {
        return sz;
    }

    static const vec3 positionsCube[] =
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
    
    TriangleMesh GetCubeMeshSolid(bool indexed)
    {
        const int ctIndices = 36;
        const GLuint indices[ctIndices] =
        {
            0, 3, 1, 0, 2, 3,
            0, 5, 4, 0, 1, 5,
            0, 4, 6, 0, 6, 2,
            7, 1, 3, 7, 5, 1,
            7, 3, 2, 7, 2, 6,
            7, 4, 5, 7, 6, 4
        };
        const int ctVertices = size(positionsCube);

        TriangleMesh ret(GL_TRIANGLES);

        for(int i = 0; i < ctIndices; ++i)
            ret.mPositions.push_back(positionsCube[indices[i]]);

        const int ctPos = ret.mPositions.size();
        for(int i = 0; i < ctPos - 2; i += 3)
        {
            const vec3 &x1 = ret.mPositions[i + 0];
            const vec3 &x2 = ret.mPositions[i + 1];
            const vec3 &x3 = ret.mPositions[i + 2];

            const vec3 normal = normalize(cross(x3 - x1, x3 - x2));
            ret.mNormals.push_back(normal);
            ret.mNormals.push_back(normal);
            ret.mNormals.push_back(normal);
        }

        if(indexed) {
            IndexMesh(ret);
        }

        return ret;
    }


    TriangleMesh GetCubeMeshWire(bool indexed)
    {
        const int ctIndices = 24;
        const GLuint indices[ctIndices] =
        {
            0, 1, 0, 2, 0, 4,
            3, 1, 3, 2, 3, 7,
            5, 1, 5, 7, 5, 4,
            6, 2, 6, 7, 6, 4
        };
        const int ctVertices = size(positionsCube);

        TriangleMesh ret(GL_LINES);
        if(indexed) {
            ret.mIndices.insert(ret.mIndices.begin(), indices, indices + ctIndices);
            ret.mPositions.insert(ret.mPositions.begin(), positionsCube, positionsCube + ctVertices);
        }
        else {
            for(int i = 0; i < ctIndices; ++i) {
                ret.mPositions.push_back(positionsCube[indices[i]]);
            }
        }

        return ret;
    }

    TriangleMesh GetSphereMesh(bool indexed)
    {
        return TriangleMesh(GL_TRIANGLES);
    }

}
