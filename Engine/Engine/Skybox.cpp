
#include <Engine/Engine/Skybox.h>
#include <Engine/Geometry/GeometryMeshes.h>

using namespace std;
using namespace glm;

namespace engine
{
    Skybox::Skybox(void)
        //: cubemap(GL_TEXTURE_CUBE_MAP)
    {
    }

    void Skybox::Load(const string &path, const string &name, const string &extension)
    {
        // Init geometry
        const TriangleMesh cube = GetCubeMeshSolid(true, true);
        mVAO.Init(&cube);

        const string side_names[] = {
            "front",
            "back",
            "bottom",
            "top",
            "left",
            "right"
        };

        const GLenum sides[] = {
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X
        };

        //glActiveTexture(GL_TEXTURE2);
        //for(int i = 0; i < 6; ++i)
        //{
        //    const string filename = path + name + side_names[i] + extension;
        //    cubemap.LoadFromFile(filename.c_str(), sides[i], false);
        //}
        //cubemap.TexParami(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //cubemap.TexParami(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //cubemap.TexParami(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        //cubemap.TexParami(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //cubemap.TexParami(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //cubemap.TexParami(GL_TEXTURE_BASE_LEVEL, 0);
        //cubemap.TexParami(GL_TEXTURE_MAX_LEVEL, 0);

        //p.SetUniform("cube_map", 2);
    }

    void Skybox::Destroy()
    {
        mVAO.Destroy();
        mProgram.Destroy();
        //cubemap.Destroy();
    }

    void Skybox::Draw(const RenderingContext &rContext) const
    {
        mProgram.Use();
        mProgram.SetUniform("VP", rContext.mVP);
        mVAO.Render(GL_TRIANGLES);
    }
}
