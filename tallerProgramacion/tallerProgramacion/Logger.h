#pragma once

#include <string>
#include <map>
#include <stddef.h>

using namespace std;

enum LogMode { Error = 0, Actividad = 1, Debug = 2 };

const std::string LOG_FILENAME_FORMAT = "log-%Y%m%d.txt";

const std::string DEBUG_SERVER_TAG = "ServerConfig";
const std::string DEBUG_CLIENT_TAG = "ClientConfig";

class Logger
{
public:
	LogMode mode;
	static Logger* getInstance();
	void setMode(LogMode mode);
	LogMode getMode();
	void log(LogMode mode, string format);
	~Logger();

private:
	Logger();
	static Logger* instance;
	string currentDateTime(std::string dateFormat);
	bool canWrite(LogMode mode);
	map<LogMode, string> mapMode;
	

};

