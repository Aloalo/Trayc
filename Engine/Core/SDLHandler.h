/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_SDLHANDLER_H
#define EN_SDLHANDLER_H

#include "SDL.h"
#include <GL/glew.h>

namespace engine
{
    //handles SDL and GLEW contexts and stuff
    class SDLHandler
    {
    public:

        static void PrintSoftwareVersions();

        static void Init(Uint32 flags);
        static void CreateGLWindow(const char* title, int x, int y, int w, int h, Uint32 flags);
        static void InitGL(int verionMajor, int versionMinor, int profile);
        static void SwapBuffers();

        static void GetWindowSize(int &w, int &h);
        static void SetMousePositionToWindowCenter();

        static void CleanUp();

    private:
        static SDL_Window *window;
        static SDL_GLContext opengl_context;
    };
}

#endif
