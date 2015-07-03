/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Utils/Profiler.h>
#include <ctime>

using namespace std;

namespace engine
{
    void Profiler::StartClock(const std::string &targetName)
    {
        mClockStarts[targetName] = clock();
    }

    void Profiler::StopClock(const std::string &targetName)
    {
        deque<int> &targetFrameLengths = mProfileTargets[targetName];
        targetFrameLengths.push_back(int(clock()) - mClockStarts[targetName]);
        targetFrameLengths.pop_front();
    }

    void Profiler::AddProfileTarget(const std::string &targetName, int ctFrames)
    {
        mProfileTargets[targetName] = deque<int>(ctFrames, 0);
    }

    float Profiler::GetTargetAverage(const std::string &targetName) const
    {
        int sum = 0;
        const deque<int> &targetFrameLengths = mProfileTargets.find(targetName)->second;

        for(int frameTime : targetFrameLengths)
            sum += frameTime;
        return float(sum) / float(targetFrameLengths.size());
    }

};
