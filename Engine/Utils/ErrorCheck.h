
#ifndef EN_ERRORCHECK_H
#define EN_ERRORCHECK_H

namespace engine
{
    #define PRINT_GL_ERRORS true
    #define PRINT_SDL_ERRORS true
    #define PRINT_GLEW_ERRORS true

    #define SDLErrCheck(x) \
        if(PRINT_SDL_ERRORS && x != 0) \
        { \
            LOG(ERROR) << "[SDLErrCheck] SDL Error: " << SDL_GetError(); \
        } \

    #define GLEWErrCheck(x) \
        if (PRINT_GLEW_ERRORS && x != GLEW_OK) \
        { \
            LOG(ERROR) << "[GLEWErrCheck] GLEW Error: " << glewGetErrorString(x); \
        } \

    void CheckOpenGLError(const char *file, int line);

        //checks for ogl errors in debug mode, unless forsed is set to true
        #ifndef NDEBUG
            #define GLErrCheck(forsed) CheckOpenGLError(__FILE__, __LINE__)
        #else
            #define GLErrCheck(forsed) \
                if(forsed) \
                { \
                    CheckOpenGLError(__FILE__, __LINE__); \
                } \

        #endif

    #define GLCheckStmt(x) \
        GLErrCheck(true); \
        x; \
        GLErrCheck(true); \


}
#endif
