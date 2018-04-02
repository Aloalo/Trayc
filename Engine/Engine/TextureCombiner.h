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

        void Init(const char *progName, const Shader::Defines &defines = Shader::Defines());
        void Init(const char *vsName, const char *fsName, const Shader::Defines &defines);
        void Init(const std::string &vsName, const std::string &fsName, const Shader::Defines &defines);
        void Destroy();

        // Get program
        const Program& Prog() const;
        static void SetTexture(int idx, const Texture *tex);

        void Draw() const;

        static void DestroyVAO();

    private:
        void Init();

        Program mProgram;
        static VertexArray mVAO;
    };
}

#endif
