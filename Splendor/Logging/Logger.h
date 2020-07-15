#pragma once
#include <iostream>
#include <string>

#ifdef LOGGING_EXPORTS
#define API_LOGGER __declspec(dllexport)
#else
#define API_LOGGER __declspec(dllimport)
#endif 

class API_LOGGER Logger
{

public:
	enum class Level
	{
		Debug,
		Info,
		Warning,
		Win,
		Error
	};

public:
	Logger(std::ostream& out, Level minLogLevel);
	void Log(const std::string& message, Level level) const;

private:
	std::ostream& m_out;
	Level m_minLogLevel;
};

