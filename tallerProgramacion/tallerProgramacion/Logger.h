#pragma once

#include <string>
#include <map>

using namespace std;

enum LogMode { ERROR = 0, APPLICATION = 1, DEBUG = 2 };

class Logger
{
public:

	LogMode mode;
	Logger();
	void setMode(LogMode mode);
	void log(LogMode mode, string message);
	~Logger();

private:
	string currentDateTime();
	bool canWrite(LogMode mode);
	map<LogMode, string> mapMode;

};

