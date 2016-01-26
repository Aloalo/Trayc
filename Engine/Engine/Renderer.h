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

        //Renderables
        void AddRenderable(Renderable *renderable);
        void RemoveRenderable(Renderable *renderable);

        //Lights
        void AddLight(const Light &light);
        void RemoveLight(int idx);

        void SetScreenSize(int width, int height);

    private:
        friend class Game;
        //Rendering
        void InitRendering(const CameraHandler *camera);
        void Render() const;

        const CameraHandler *mCamera;
        Game *mGame;

        GLbitfield mClearMask;
        std::vector<Renderable*> mRenderables;

        // Deferred rendering stuff
        engine::FrameBuffer mGBuffer;
        std::map<std::string, Program> mMatToProg;
    };
}

#endif
