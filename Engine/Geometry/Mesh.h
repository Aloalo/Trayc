/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_MESH_H
#define EN_MESH_H

#include <Engine/Geometry/IndexContainer.h>
#include <Engine/Geometry/VertexAttributeArray.h>
#include <vector>

namespace engine
{
    class Mesh
    {
    public:
        Mesh(void);
        Mesh(const std::vector<VertexAttributeArray> &attributeArrays);
        ~Mesh(void);

        void SetIndices(const std::vector<GLuint> &indices);
        void SetIndices(const IndexContainer &indices);
        void AddAttributeArray(const VertexAttributeArray &attrib);

        //returns the index of the wanted attribute array
        //or -1 if it doesnt exist
        int GetAttribArrayIdx(AttributeType type) const;
        const VertexAttributeArray& GetAttribArray(AttributeType type) const;
        VertexAttributeArray& GetAttribArray(AttributeType type);
        const IndexContainer& GetIndexContainer() const;

    private:
        std::vector<VertexAttributeArray> attribArrays;
        IndexContainer indexData;

        GLenum orientation;
    };
}

#endif
