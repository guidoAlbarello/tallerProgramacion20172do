#include "Logger.h"
#include <string>
#include <iostream>
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;


Logger::Logger()
{
	mapMode[DEBUG] = "Debug";
	mapMode[APPLICATION] = "Application";
	mapMode[ERROR] = "Error";
	this->mode = DEBUG;
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
