#ifndef EN_CONTEXT_HANDLER_H
#define EN_CONTEXT_HANDLER_H

#include "SDL.h"
#include <GL/glew.h>

namespace engine
{
    //handles SDL and GLEW contexts and stuff
    class ContextHandler
    {
    public:
        ContextHandler(void);
        ~ContextHandler(void);

        void PrintSoftwareVersions() const;
        void SwapBuffers();

        void GetWindowSize(int *w, int *h);
        void SetWindowSize(int w, int h);

        //0 -> off, 1-> on
        void VsyncMode(int mode) const;

        const SDL_version& GetLinkedVersion() const;
        const SDL_version& GetCompiledVersion() const;
        char const* GetProgramName() const;

    private:
        friend class Game;
        void Init(Uint32 flags, const char *programName);
        void CreateGLWindow(const char* title, int x, int y, int w, int h, Uint32 flags);
        void InitGL(int verionMajor, int versionMinor, int profile);
        void InitIL();


        SDL_version mCompiled;
        SDL_version mLinked;
        char const *mProgramName;

        SDL_Window *mWindow;
        SDL_GLContext mOpenglContext;
    };
}

#endif
