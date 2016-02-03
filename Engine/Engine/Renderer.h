/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_RENDERER_H
#define EN_RENDERER_H

#include <Engine/Engine/Renderable.h>
#include <Engine/Engine/Light.h>
#include <Engine/GL/FrameBuffer.h>
#include <Engine/GL/Program.h>
#include <Engine/GL/VertexArray.h>
#include <Engine/Geometry/Scene.h>

#include <Engine/Engine/GlobalRenderingParams.h>

namespace engine
{
    class Game;
    class CameraHandler;

    class Renderer
    {
    public:
        Renderer(Game *scene);
        ~Renderer(void);

        void SetClearColor(const glm::vec4 &clearColor) const;

        void SetScene(const Scene *scene);

        //Renderables
        void AddRenderable(Renderable *renderable);
        void RemoveRenderable(Renderable *renderable);

        void SetScreenSize(int width, int height);

    private:
        friend class Game;
        //Rendering
        void InitRendering(const CameraHandler *camera);
        void Render() const;

        void ClearVertexArrays();
        void InitScene(const Scene *scene);

        const Scene *mScene;
        const CameraHandler *mCamera;
        Game *mGame;

        GLbitfield mClearMask;
        std::vector<Renderable*> mRenderables;

        // Deferred rendering stuff
        FrameBuffer mGBuffer;
        Program mGPrograms[1 << MatTextureType::CT_MAT_TEXTURE_TYPES];
        std::map<std::string, Texture2D> mNameToTex;
        // At index i is VA for mesh i
        std::vector<VertexArray> mVertexArrays;
    };
}

#endif
