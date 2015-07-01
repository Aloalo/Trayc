/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Core/SDLHandler.h>
#include <Engine/Utils/ErrorCheck.h>
#include <iostream>

using namespace std;

namespace engine
{
    SDLHandler::SDLHandler(void)
        : mProgramName(nullptr), mWindow(nullptr)
    {
    }

    void SDLHandler::Init(Uint32 flags, const char *programName)
    {
        SDLErrCheck(SDL_Init(flags));
        SDLErrCheck(SDL_SetRelativeMouseMode(SDL_TRUE));
        SDL_VERSION(&mCompiled);
        SDL_GetVersion(&mLinked);
        mProgramName = programName;
    }

    void SDLHandler::CleanUp()
    {
        SDL_GL_DeleteContext(mOpenglContext);
        SDL_DestroyWindow(mWindow);
        SDL_Quit();
    }

    void SDLHandler::PrintSoftwareVersions() const
    {
        cout << "GLEW version: " << glewGetString(GLEW_VERSION) << endl << endl;

        cout << "Compiled against SDL version "  << static_cast<int>(mCompiled.major) << "." << 
                                                     static_cast<int>(mCompiled.minor)  << "." <<  
                                                     static_cast<int>(mCompiled.patch) << endl;
        cout << "Linked against SDL version "  << static_cast<int>(mLinked.major) << "." << 
                                                   static_cast<int>(mLinked.minor)  << "." <<  
                                                   static_cast<int>(mLinked.patch) << endl << endl;

        cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << endl;
        cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << endl;
        cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
        cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl << endl;
    }

    void SDLHandler::CreateGLWindow(const char *title, int x, int y, int w, int h, Uint32 flags)
    {
        mWindow = SDL_CreateWindow(title, x, y, w, h, flags);
        SDLErrCheck(!mWindow);
    }

    void SDLHandler::InitGL(int verionMajor, int versionMinor, int profile)
    {
        SDLErrCheck(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, verionMajor));
        SDLErrCheck(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, versionMinor));
        SDLErrCheck(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, profile));

        SDLErrCheck(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));

        // create the opengl context
        mOpenglContext = SDL_GL_CreateContext(mWindow);
        SDLErrCheck(!mOpenglContext);
    
        //SDLErrCheck(SDL_GL_SetSwapInterval(1));

        glewExperimental = GL_TRUE;
        GLEWErrCheck(glewInit());
        GLErrCheck(true);
    }

    void SDLHandler::SwapBuffers()
    {
        SDL_GL_SwapWindow(mWindow);
    }

    void SDLHandler::GetWindowSize(int *w, int *h)
    {
        SDL_GetWindowSize(mWindow, w, h);
    }

    void SDLHandler::SetWindowSize(int w, int h)
    {
        SDL_SetWindowSize(mWindow, w, h);
    }

    const SDL_version& SDLHandler::GetLinkedVersion() const
    {
        return mLinked;
    }

    const SDL_version& SDLHandler::GetCompiledVersion() const
    {
        return mCompiled;
    }

    char const* SDLHandler::GetProgramName() const
    {
        return mProgramName;
    }
}
