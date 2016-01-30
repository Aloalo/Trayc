#ifndef EN_VERTEX_ATTRIBUTE_DEF_H
#define EN_VERTEX_ATTRIBUTE_DEF_H

#include <GL/glew.h>

namespace engine
{
    struct VertexAttribDef
    {
        VertexAttribDef(GLint index, GLint size, GLenum type, GLboolean normalized);

        int SizeInBytes() const;

        GLuint index;
        GLint size;
        GLenum type;
        GLboolean normalized;
    };
}

#endif

