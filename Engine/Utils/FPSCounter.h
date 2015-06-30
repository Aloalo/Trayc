/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_FPSCOUNTER_H
#define EN_FPSCOUNTER_H

#include <deque>

namespace engine
{
	class FPSCounter
	{
	public:
        //Number of frames to average
        FPSCounter(int ctFrames);

        void StartClock();
        void StopClock();
        float GetFPS() const;

	private:
		std::deque<int> mFrameLengths;
        int mClockStart;
	};
}

#endif
