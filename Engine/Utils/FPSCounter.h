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
        static void StartClock();
        static void StopClock();
        static float GetFPS();

	private:
		static std::deque<int> frameLengths;
        static int clockStart;
	};
}

#endif
