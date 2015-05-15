/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Utils/FPSCounter.h>
#include <ctime>

using namespace std;

namespace engine
{
    int FPSCounter::clockStart = clock();
    deque<int> FPSCounter::frameLengths = deque<int>(30, 0);

    void FPSCounter::StartClock()
    {
        clockStart = clock();
    }

    void FPSCounter::StopClock()
    {
        frameLengths.push_back(clock() - clockStart);
        frameLengths.pop_front();
    }

    float FPSCounter::GetFPS()
    {
        float FPS = 0.0f;
        for(int frameLength : frameLengths)
            FPS += float(frameLength);
        FPS /= float(frameLengths.size());
        return float(CLOCKS_PER_SEC) / FPS;
    }
};
