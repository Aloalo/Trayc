#include <Engine/GL/VertexAttributeDef.h>
#include <Engine/Utils/Utilities.h>

namespace engine
{
    VertexAttribDef::VertexAttribDef(GLint index, GLint size, GLenum type, GLboolean normalized) :
        index(index), size(size), type(type), normalized(normalized)
    {
    }

    int VertexAttribDef::SizeInBytes() const
    {
        return size * SizeOfGLType(type);
    }
}
