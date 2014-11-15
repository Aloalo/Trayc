/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Geometry/Mesh.h>
#include <Engine/Common/MathFunctions.h>
#include <map>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

namespace engine
{
    Mesh::Mesh(void)
        : orientation(GL_CCW)
    {
    }

    Mesh::Mesh(const std::vector<VertexAttributeArray> &attributeArrays)
        : attribArrays(attributeArrays), orientation(GL_CCW)
    {
    }


    Mesh::~Mesh(void)
    {
    }

    void Mesh::SetIndices(const std::vector<GLuint> &indices)
    {
        indexData = IndexContainer(indices);
    }

    void Mesh::SetIndices(const IndexContainer &indices)
    {
        indexData = indices;
    }

    void Mesh::AddAttributeArray(const VertexAttributeArray &attrib)
    {
        attribArrays.push_back(attrib);
    }
    
    int Mesh::GetAttribArrayIdx(AttributeType type) const
    {
        const int ctArrays = attribArrays.size();
        for(int i = 0; i < ctArrays; ++i)
            if(attribArrays[i].GetVertexAttrib().attribType == type)
                return i;
        return -1;
    }

    const VertexAttributeArray& Mesh::GetAttribArray(AttributeType type) const
    {
        return attribArrays[GetAttribArrayIdx(type)];
    }

    VertexAttributeArray& Mesh::GetAttribArray(AttributeType type)
    {
        return attribArrays[GetAttribArrayIdx(type)];
    }

    const IndexContainer& Mesh::GetIndexContainer() const
    {
        return indexData;
    }
}
