/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Utils/Profiler.h>
#include <ctime>
#include <iostream>

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

    float Profiler::GetAverage() const
    {
        float sum = 0.0f;
        for(const auto &si : mProfileTargets)
            sum += GetTargetAverage(si.first);
        return sum;
    }

    using psf = pair<string, float>;
    vector<psf> Profiler::GetAllTargetsAverage() const
    {
        vector<psf> ret;
        for(const auto &si : mProfileTargets)
            ret.push_back(psf(si.first, GetTargetAverage(si.first)));
        return ret;
    }

    void Profiler::PrintProfile() const
    {
        float sum = 0.0f;
        cout << "------PROFILE------" << endl;
        for(const auto &psf : GetAllTargetsAverage())
        {
            cout << psf.first + ": " << psf.second << endl;
            sum += psf.second;
        }
        cout << "NET: " << sum << endl << endl;
    }

};
