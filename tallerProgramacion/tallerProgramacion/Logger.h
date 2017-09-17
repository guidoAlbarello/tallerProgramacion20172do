#pragma once
#ifndef  LOGGER_H
#define LOGGER_H

#include <string>
#include <map>
#include <stddef.h>
#include <mutex>

using namespace std;

enum LogMode { Error = 0, Actividad = 1, Debug = 2 };

const std::string DEBUG_SERVER_TAG = "ServerConfig";
const std::string DEBUG_CLIENT_TAG = "ClientConfig";

class Logger
{
public:
	LogMode mode;
	static Logger* getInstance();
	void setMode(LogMode mode);
	LogMode getMode();
	void setLogFileName(std::string fileNameFormat);
	void log(LogMode mode, string format);
	void log(LogMode mode, char *message);
	~Logger();

private:
	Logger();
	static Logger* instance;
	string currentDateTime(std::string dateFormat);
	bool canWrite(LogMode mode);
	map<LogMode, string> mapMode;
	mutex m_loggerMutex;
	std::string logFileName;

};

#endif