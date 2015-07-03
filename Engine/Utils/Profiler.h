/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_FPSCOUNTER_H
#define EN_FPSCOUNTER_H

#include <deque>
#include <string>
#include <map>

namespace engine
{
	class Profiler
	{
	public:
        //Name of the profile an the number of frames to average
        void AddProfileTarget(const std::string &targetName, int ctFrames);

        void StartClock(const std::string &targetName);
        void StopClock(const std::string &targetName);
        //Return the average number of miliseconds over ctFrames for target
        float GetTargetAverage(const std::string &targetName) const;

	private:
        std::map<std::string, std::deque<int>> mProfileTargets;
        std::map<std::string, int> mClockStarts;
	};
}

#endif
