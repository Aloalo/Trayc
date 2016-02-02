/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_RENDERER_H
#define EN_RENDERER_H

#include <Engine/Engine/Renderable.h>
#include <Engine/Engine/Light.h>
#include <Engine/Core/CameraHandler.h>
#include <Engine/GL/FrameBuffer.h>
#include <Engine/GL/Program.h>
#include <Engine/GL/VertexArray.h>
#include <Engine/Geometry/Scene.h>
#include <vector>

namespace engine
{
    class Game;

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
        std::map<std::string, Program> mMatToProg;
        // At index i is VA for mesh i
        std::vector<VertexArray> mVertexArrays;
    };
}

#endif
