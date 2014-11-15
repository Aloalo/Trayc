/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_INITIALSETTINGS_H
#define EN_INITIALSETTINGS_H

#include <vector>
#include <string>
#include <map>

namespace engine
{
	struct InitialSettings
	{
		unsigned int& operator[](const std::string &variableName);

		static InitialSettings& get();

	private:
		InitialSettings(const std::string &path);

		static InitialSettings *instance;

		std::map<std::string, unsigned int> values;
		std::map<std::string, unsigned int> format;
	};
}

#endif
