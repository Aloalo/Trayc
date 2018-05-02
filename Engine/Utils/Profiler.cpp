
#include <Engine/Utils/Profiler.h>
#include <easylogging++.h>
#include <chrono>

using namespace std;

namespace engine
{
    using Clock = chrono::high_resolution_clock;

    void Profiler::StartClock(const std::string &targetName)
    {
#if PROFILE_CPU
        const int time = static_cast<int>(clock());

        if (mProfileTargets.find(targetName) == mProfileTargets.end()) {
            const int ctFrames = 100;
            mProfileTargets[targetName] = deque<int>(ctFrames, 0);
            mTargetOrder.push_back(targetName);
        }

        mClockStarts[targetName] = time;
#endif
    }

    void Profiler::StopClock(const std::string &targetName)
    {
#if PROFILE_CPU
        const int time = static_cast<int>(clock());

        deque<int> &targetFrameLengths = mProfileTargets[targetName];
        targetFrameLengths.push_back(time - mClockStarts[targetName]);
        targetFrameLengths.pop_front();
#endif
    }

    void Profiler::PrintProfile() const
    {
#if PROFILE_CPU
        float sum = 0.0f;
        CLOG(INFO, "performance") << "------PROFILE------";
        for (const auto &psf : GetAllTargetsAverage())
        {
            CLOG(INFO, "performance") << psf.first + ": " << psf.second;
            sum += psf.second;
        }
        CLOG(INFO, "performance") << "NET: " << sum << endl;
#endif
    }

#if PROFILE_CPU
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
        for (const string &t : mTargetOrder) {
            ret.push_back(psf(t, GetTargetAverage(t)));
        }
        return ret;
    }
#endif
}
