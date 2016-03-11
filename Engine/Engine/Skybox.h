#ifndef EN_SKYBOX_H
#define EN_SKYBOX_H

#include <GL/glew.h>
#include <Engine/Engine/Renderable.h>
#include <Engine/GL/Texture2D.h>
#include <Engine/GL/Program.h>
#include <Engine/GL/VertexArray.h>
#include <string>

namespace engine
{
    class Skybox : public Renderable
    {
    public:
        Skybox(void);

        //" left", "right", "top", "bottom", "far" and "near" are added to name for each side
        void Load(const std::string &path, const std::string &name, const std::string &extension);
        void Destroy();
        void Draw(const RenderingContext &rContext) const override;

    private:
        //Texture cubemap;
        Program mProgram;
        VertexArray mVAO;
    };
}

#endif
