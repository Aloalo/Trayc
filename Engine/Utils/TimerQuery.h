#ifndef EN_TIMER_QUERY_H
#define EN_TIMER_QUERY_H

#include <GL/glew.h>
#include <Engine/Core/Defines.h>
#include <string>

namespace engine
{
    class TimerQuery
    {
    public:
        TimerQuery(const std::string &name);
        ~TimerQuery(void);

    private:
#if PROFILE_GPU
        GLuint mIDs[2];
        std::string mName;
#endif
    };
}

#endif