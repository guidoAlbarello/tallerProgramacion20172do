#include "Logger.h"
#include <string>
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

	std::ofstream logFile;
	m_loggerMutex.lock();
	logFile.open(currentDateTime(LOG_FILENAME_FORMAT), ofstream::app);
	logFile << currentDateTime("%d-%m-%Y %I:%M:%S") << " - " << mapMode[mode] << " - " << message << std::endl;
	logFile.close();
	m_loggerMutex.unlock();
}

void Logger::log(LogMode mode, char *message) {
	if (message != NULL) {
		std::string mensaje(message);
		log(mode, mensaje);
	} 
	else {
		std:string vacio = "pidieron logear un mensaje vacio";
		log(mode, vacio);
	}

}
void Logger::setMode(LogMode mode)
{
	this->mode = mode;
}

LogMode Logger::getMode()
{
	return this->mode;
}

string Logger::currentDateTime(std::string dateFormat) {
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);
	char buffer[80];

	strftime(buffer, sizeof(buffer), dateFormat.c_str(), &newtime);
	std::string str(buffer);

	return str;
}

bool Logger::canWrite(LogMode mode) {
	return mode <= this->mode;

}

Logger *Logger::getInstance() {
	if (!instance) {
		instance = new Logger();
		instance->setMode(LogMode::Debug);
	}

	return instance;
}
