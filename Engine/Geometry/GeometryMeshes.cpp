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

    void CalculateNormals(TriangleMesh &mesh)
    {
        const int ctPos = mesh.mPositions.size();
        for(int i = 0; i < ctPos - 2; i += 3)
        {
            const vec3 &x1 = mesh.mPositions[i + 0];
            const vec3 &x2 = mesh.mPositions[i + 1];
            const vec3 &x3 = mesh.mPositions[i + 2];

            const vec3 normal = normalize(cross(x3 - x1, x3 - x2));
            mesh.mNormals.push_back(normal);
            mesh.mNormals.push_back(normal);
            mesh.mNormals.push_back(normal);
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
    
    TriangleMesh GetCubeMeshSolid(bool indexed, bool normals)
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

        TriangleMesh ret(GL_TRIANGLES);

        if(normals)
        {
            for(int i = 0; i < ctIndices; ++i) {
                ret.mPositions.push_back(positionsCube[indices[i]]);
            }
            CalculateNormals(ret);
            if(indexed) {
                IndexMesh(ret);
            }
        }
        else
        {
            ret.mPositions.insert(ret.mPositions.begin(), positionsCube, positionsCube+8);
            if(indexed) {
                ret.mIndices.insert(ret.mIndices.begin(), indices, indices+36);
            }
        }

        ret.CalcAABB();
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

        ret.CalcAABB();
        return ret;
    }

    static void SphereSubdivide(const vec3 &a, const vec3 &b, const vec3 &c, int depth, TriangleMesh &out)
    {
        if(depth == 0)
        {
            out.mPositions.push_back(c);
            out.mPositions.push_back(b);
            out.mPositions.push_back(a);

            out.mNormals.push_back(c);
            out.mNormals.push_back(b);
            out.mNormals.push_back(a);
            return;
        }

        const vec3 ab = normalize(a + b);
        const vec3 bc = normalize(b + c);
        const vec3 ca = normalize(c + a);

        SphereSubdivide(a, ab, ca, depth - 1, out);
        SphereSubdivide(b, bc, ab, depth - 1, out);
        SphereSubdivide(c, ca, bc, depth - 1, out);
        SphereSubdivide(ab, bc, ca, depth - 1, out);
    }

    TriangleMesh GetSphereMeshSolid(bool indexed, int quality)
    {
        TriangleMesh sphere(GL_TRIANGLES);

        const float X = 0.525731112119133606f;
        const float Z = 0.850650808352039932f;

        const vec3 positions[12] =
        {
            vec3(-X, 0.0, Z), vec3(X, 0.0, Z), vec3(-X, 0.0, -Z), vec3(X, 0.0, -Z),
            vec3(0.0, Z, X), vec3(0.0, Z, -X), vec3(0.0, -Z, X), vec3(0.0, -Z, -X),
            vec3(Z, X, 0.0), vec3(-Z, X, 0.0), vec3(Z, -X, 0.0), vec3(-Z, -X, 0.0)
        };

        const unsigned int indices[20][3] =
        {
            { 0,4,1 },{ 0,9,4 },{ 9,5,4 },{ 4,5,8 },{ 4,8,1 },
            { 8,10,1 },{ 8,3,10 },{ 5,3,8 },{ 5,2,3 },{ 2,7,3 },
            { 7,10,3 },{ 7,6,10 },{ 7,11,6 },{ 11,0,6 },{ 0,1,6 },
            { 6,1,10 },{ 9,0,11 },{ 9,11,2 },{ 9,2,5 },{ 7,2,11 }
        };

        for(int i = 0; i < 20; i++) {
            SphereSubdivide(positions[indices[i][0]], positions[indices[i][1]], positions[indices[i][2]], quality, sphere);
        }

        if(indexed) {
            IndexMesh(sphere);
        }

        sphere.CalcAABB();
        return sphere;
    }

}
