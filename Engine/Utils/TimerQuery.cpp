#include <Engine/Utils/TimerQuery.h>
#include <easylogging++.h>
#include <map>
#include <deque>

using namespace std;

namespace engine
{
#if PROFILE_GPU
    static map<string, deque<float>> targets;
    static map<string, int> counters;
    static const int frameCap = 100;

    TimerQuery::TimerQuery(const std::string &name)
        : mName(name)
    {
        glGenQueries(2, mIDs);
        glQueryCounter(mIDs[0], GL_TIMESTAMP);

        if (targets.find(name) == targets.end()) {
            const int ctFrames = 100;
            targets[name] = deque<float>(ctFrames, 0.0f);
            counters[name] = 0;
        }
    }

    TimerQuery::~TimerQuery(void)
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

        const float dt = static_cast<float>(stopTime - startTime) / 1000000.0f;

        // Compute the average
        auto &times = targets[mName];
        times.push_back(dt);
        times.pop_front();
        int &count = counters[mName];
        ++count;

        if (count == frameCap) {
            count = 0;

            float avg = 0.0f;

            for (float t : times) {
                avg += t;
            }
            avg /= static_cast<float>(times.size());
            CLOG(INFO, "performance") << "    " << mName << ": " << avg;
        }

        glDeleteQueries(2, mIDs);
    }
#else
    TimerQuery::TimerQuery(const std::string &name)
    {
    }

    TimerQuery::~TimerQuery(void)
    {
    }
#endif
}
