#pragma once

#include <string>
#include <map>
#include <stddef.h>

using namespace std;

enum LogMode { Error = 0, Actividad = 1, Debug = 2 };

class Logger
{
public:
	LogMode mode;
	static Logger* getInstance();
	void setMode(LogMode mode);
	void log(LogMode mode, string message);
	~Logger();

private:
	Logger();
	static Logger* instance;
	string currentDateTime();
	bool canWrite(LogMode mode);
	map<LogMode, string> mapMode;
	

};

