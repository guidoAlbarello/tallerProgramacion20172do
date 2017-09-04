#include "Logger.h"
#include <string>
#include <iostream>
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;
Logger* Logger::instance = NULL;

Logger::Logger()
{
	mapMode[Debug] = "Debug";
	mapMode[Actividad] = "Actividad";
	mapMode[Error] = "Error";
	//this->mode = Debug;
}


Logger::~Logger()
{
}

void Logger::log(LogMode mode, string message)
{
	if (!canWrite(mode)) {
		return;
	}

	ofstream myfile;
	myfile.open("log.txt", ofstream::app);
	myfile << currentDateTime() << " - " << mapMode[mode] << " - " << message << "\n";
	myfile.close();
}

void Logger::setMode(LogMode mode)
{
	this->mode = mode;
}

LogMode Logger::getMode()
{
	return this->mode;
}

string Logger::currentDateTime() {
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);
	char buffer[80];

	strftime(buffer, sizeof(buffer), "%d-%m-%Y %I:%M:%S", &newtime);
	std::string str(buffer);

	return str;
}

bool Logger::canWrite(LogMode mode) {
	return mode <= this->mode;

}

Logger *Logger::getInstance() {
	if (!instance) {
		instance = new Logger();
		instance->setMode(LogMode::Actividad);
	}

	return instance;
}
