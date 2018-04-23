#ifndef EN_TIMER_QUERY_H
#define EN_TIMER_QUERY_H

#include <GL/glew.h>

namespace engine
{
    class TimerQuery
    {
    public:
        TimerQuery(void);
        ~TimerQuery(void);

        float Stop() const;

    private:
        GLuint mIDs[2];
    };
}

#endif