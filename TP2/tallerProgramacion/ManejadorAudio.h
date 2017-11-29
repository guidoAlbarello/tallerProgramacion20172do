#pragma once
#ifndef MANEJADORAUDIO_H
#define MANEJADORAUDIO_H

#include <SDL_mixer.h>
#include <map>
#include <SDL.h>
#include "Logger.h"

class ManejadorAudio
{
public:
	ManejadorAudio();
	~ManejadorAudio();
	static ManejadorAudio* getInstance();
	bool load(std::string fileName, std::string	id);
	bool startOrPauseTrack(std::string id);
	bool pauseTrack();
	bool startTrack(std::string id);
	bool playTrackOnce(std::string id);
private:
	static ManejadorAudio* instance;
	std::map<std::string, Mix_Music*> sonidos;
};

#endif