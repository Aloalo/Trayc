#include <Engine/Core/ContextHandler.h>
#include <Engine/Utils/ErrorCheck.h>

#include <easylogging++.h>

#include <glm/glm.hpp>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

using namespace std;

namespace engine
{
    ContextHandler::ContextHandler(void)
        : mProgramName(nullptr), mWindow(nullptr)
    {
    }

    ContextHandler::~ContextHandler(void)
    {
        SDL_GL_DeleteContext(mOpenglContext);
        SDL_DestroyWindow(mWindow);
        SDL_Quit();
    }

    void ContextHandler::Init(Uint32 flags, const char *programName)
    {
        SDLErrCheck(SDL_Init(flags));
        SDLErrCheck(SDL_SetRelativeMouseMode(SDL_TRUE));
        SDL_VERSION(&mCompiled);
        SDL_GetVersion(&mLinked);
        mProgramName = programName;
    }

    void ContextHandler::PrintSoftwareVersions() const
    {
        LOG(INFO) << "[ContextHandler::PrintSoftwareVersions] GLEW version: " << glewGetString(GLEW_VERSION) << endl;

        LOG(INFO) << "[ContextHandler::PrintSoftwareVersions] Compiled against SDL version "  << 
                                                     static_cast<int>(mCompiled.major) << "." <<
                                                     static_cast<int>(mCompiled.minor)  << "." <<
                                                     static_cast<int>(mCompiled.patch);
        LOG(INFO) << "[ContextHandler::PrintSoftwareVersions] Linked against SDL version "  << 
                                                    static_cast<int>(mLinked.major) << "." <<
                                                    static_cast<int>(mLinked.minor)  << "." <<
                                                    static_cast<int>(mLinked.patch) << endl;

        LOG(INFO) << "[ContextHandler::PrintSoftwareVersions] OpenGL vendor: " << glGetString(GL_VENDOR);
        LOG(INFO) << "[ContextHandler::PrintSoftwareVersions] OpenGL renderer: " << glGetString(GL_RENDERER);
        LOG(INFO) << "[ContextHandler::PrintSoftwareVersions] OpenGL version: " << glGetString(GL_VERSION);
        LOG(INFO) << "[ContextHandler::PrintSoftwareVersions] GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

        LOG(INFO) << "[ContextHandler::PrintSoftwareVersions] IL version: " << IL_VERSION;
        LOG(INFO) << "[ContextHandler::PrintSoftwareVersions] ILU version: " << ILU_VERSION;
        LOG(INFO) << "[ContextHandler::PrintSoftwareVersions] ILUT version: " << ILUT_VERSION << endl;

        LOG(INFO) << "[ContextHandler::PrintSoftwareVersions] GLM version: " << GLM_VERSION_MAJOR << "." << GLM_VERSION_MINOR << "." << GLM_VERSION_PATCH << endl;
    }

    void ContextHandler::CreateGLWindow(const char *title, int x, int y, int w, int h, Uint32 flags)
    {
        mWindow = SDL_CreateWindow(title, x, y, w, h, flags);
        SDLErrCheck(!mWindow);
    }

    void ContextHandler::InitGL(int verionMajor, int versionMinor, int profile)
    {
        SDLErrCheck(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, verionMajor));
        SDLErrCheck(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, versionMinor));
        SDLErrCheck(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, profile));
        SDLErrCheck(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32));

        SDLErrCheck(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));

        // create the opengl context
        mOpenglContext = SDL_GL_CreateContext(mWindow);
        SDLErrCheck(!mOpenglContext);

        glewExperimental = GL_TRUE;
        GLEWErrCheck(glewInit());
        GLErrCheck(true);
    }

    void ContextHandler::InitIL()
    {
        ilInit();
        iluInit();
    }

    void ContextHandler::VsyncMode(int mode) const
    {
        SDLErrCheck(SDL_GL_SetSwapInterval(mode));
    }

    void ContextHandler::SwapBuffers()
    {
        SDL_GL_SwapWindow(mWindow);
    }

    void ContextHandler::GetWindowSize(int *w, int *h)
    {
        SDL_GetWindowSize(mWindow, w, h);
    }

    void ContextHandler::SetWindowSize(int w, int h)
    {
        SDL_SetWindowSize(mWindow, w, h);
    }

    const SDL_version& ContextHandler::GetLinkedVersion() const
    {
        return mLinked;
    }

    const SDL_version& ContextHandler::GetCompiledVersion() const
    {
        return mCompiled;
    }

    char const* ContextHandler::GetProgramName() const
    {
        return mProgramName;
    }
}
