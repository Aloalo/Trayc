#include "TimerQuery.h"

namespace engine
{
    TimerQuery::TimerQuery(void)
    {
        glGenQueries(2, mIDs);
        glQueryCounter(mIDs[0], GL_TIMESTAMP);
    }

    TimerQuery::~TimerQuery(void)
    {
        glDeleteQueries(2, mIDs);
    }

    float TimerQuery::Stop() const
    {
        glQueryCounter(mIDs[1], GL_TIMESTAMP);

        GLint stopTimerAvailable = 0;
        while (!stopTimerAvailable) {
            glGetQueryObjectiv(mIDs[1], GL_QUERY_RESULT_AVAILABLE, &stopTimerAvailable);
        }

        // Get query results
        GLuint64 startTime, stopTime;
        glGetQueryObjectui64v(mIDs[0], GL_QUERY_RESULT, &startTime);
        glGetQueryObjectui64v(mIDs[1], GL_QUERY_RESULT, &stopTime);

        return static_cast<float>(stopTime - startTime) / 1000000.0f;
    }
}
