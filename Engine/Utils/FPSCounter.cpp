/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Utils/FPSCounter.h>
#include <ctime>

using namespace std;

namespace engine
{
    int FPSCounter::mClockStart = clock();
    deque<int> FPSCounter::mFrameLengths = deque<int>(30, 0);

    void FPSCounter::StartClock()
    {
        mClockStart = clock();
    }

    void FPSCounter::StopClock()
    {
        mFrameLengths.push_back(clock() - mClockStart);
        mFrameLengths.pop_front();
    }

    float FPSCounter::GetFPS()
    {
        float FPS = 0.0f;
        for(int frameLength : mFrameLengths)
            FPS += float(frameLength);
        FPS /= float(mFrameLengths.size());
        return float(CLOCKS_PER_SEC) / FPS;
    }
};
