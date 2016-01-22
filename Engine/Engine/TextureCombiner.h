#ifndef EN_TEXTURE_COMBINER_H
#define EN_TEXTURE_COMBINER_H

#include <Engine/GL/Program.h>
#include <Engine/GL/FrameBuffer.h>
#include <Engine/GL/VertexArray.h>
#include <vector>

namespace engine
{
    // Combines multiple textures into one using a program
    class TextureCombiner
    {
    public:
        TextureCombiner(void);
        ~TextureCombiner(void);

        void Init(const char *progName);
        void Destroy();

        // Get program
        const Program& Prog() const;
        void SetTexture(int idx, const Texture2D &tex) const;
        // To frambuffer
        void Draw(const FrameBuffer &FBO) const;
        // To back buffer
        void Draw() const;

    private:
        Program mProgram;
        VertexArray mVAO;
    };
}

#endif
