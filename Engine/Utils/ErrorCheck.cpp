
#include <Engine/Utils/ErrorCheck.h>
#include <Engine/Utils/Utilities.h>
#include <GL/glew.h>
#include <easylogging++.h>

namespace engine
{
    using namespace std;

    void CheckOpenGLError(const char *file, int line)
    {
        GLenum err(glGetError());
 
        while(err != GL_NO_ERROR)
        {
            if(PRINT_GL_ERRORS)
            {
                string error;

                switch(err)
                {
                    case GL_INVALID_OPERATION:
                        error = "INVALID_OPERATION";
                        break;
                    case GL_INVALID_ENUM:
                        error = "INVALID_ENUM";
                        break;
                    case GL_INVALID_VALUE:
                        error = "INVALID_VALUE";
                        break;
                    case GL_OUT_OF_MEMORY:
                        error = "OUT_OF_MEMORY";
                        break;
                    case GL_INVALID_FRAMEBUFFER_OPERATION:
                        error = "INVALID_FRAMEBUFFER_OPERATION";
                        break;
                }
 
                string sfile(file);
                StringReplace(sfile, "\\", "/");
                const string fname = sfile.substr(sfile.find_last_of("/")+1);
                LOG(ERROR) << "[CheckOpenGLError] GL_" << error << " - " << fname << ":" << line;
            }

            err = glGetError();
        }
    }
}
