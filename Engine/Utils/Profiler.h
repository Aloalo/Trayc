
#ifndef EN_FPSCOUNTER_H
#define EN_FPSCOUNTER_H

#include <Engine/Utils/Singleton.h>
#include <Engine/Core/Defines.h>
#include <deque>
#include <string>
#include <map>
#include <vector>

namespace engine
{
    class Profiler : public Singleton<Profiler>
    {
    public:
        void StartClock(const std::string &targetName);
        void StopClock(const std::string &targetName);
        void PrintProfile() const;

    private:
#if PROFILE_CPU
        // Return the average time in ms over ctFrames for target
        float GetTargetAverage(const std::string &targetName) const;
        float GetAverage() const;

        // Return the average time in ms over ctFrames for all targets
        std::vector<std::pair<std::string, float>> GetAllTargetsAverage() const;

        std::map<std::string, std::deque<int>> mProfileTargets;
        std::map<std::string, int> mClockStarts;
        // Name, indent
        std::vector<std::string> mTargetOrder;
#endif
    };
}

#endif
