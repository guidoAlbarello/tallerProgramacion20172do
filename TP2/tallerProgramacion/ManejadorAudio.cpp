#include "ManejadorAudio.h"

ManejadorAudio* ManejadorAudio::instance = 0;

ManejadorAudio::ManejadorAudio()
{
	this->load("sound/init_track.wav", "initTrack");
	this->load("sound/motor1.wav", "motor");
	this->load("sound/motor2.wav", "motor2");
	this->load("sound/car_crash.wav", "carCrash");
}


ManejadorAudio::~ManejadorAudio() {
	std::map<std::string, Mix_Music*>::iterator it;

	for (it = sonidos.begin(); it != sonidos.end(); it++) {
		Mix_FreeMusic(it->second);
	}
}

ManejadorAudio* ManejadorAudio::getInstance() {
	if (!instance) {
		instance = new ManejadorAudio();
	}

	return instance;
}

bool ManejadorAudio::load(std::string fileName, std::string	id) {
	Mix_Music *music = NULL;

	//Load the music
	music = Mix_LoadMUS(fileName.c_str()); // Ya debe venir con extension .wav
	//If there was a problem loading the music
	if (music == NULL)
	{
		return false;
	}

	//Load the sound effects
	//scratch = Mix_LoadWAV("scratch.wav");
	//high = Mix_LoadWAV("high.wav");
	//med = Mix_LoadWAV("medium.wav");
	//low = Mix_LoadWAV("low.wav");

	////If there was a problem loading the sound effects
	//if ((scratch == NULL) || (high == NULL) || (med == NULL) || (low == NULL))
	//{
	//	return false;
	//}

	//If everything loaded fine
	sonidos[id] = music;

	return true;

}


bool ManejadorAudio::startOrPauseTrack(std::string id) {
	try {
		Mix_Music* music = sonidos[id];

		//If there is no music playing
		if (Mix_PlayingMusic() == 0) {
			//Play the music
			if (Mix_PlayMusic(music, -1) == -1) {
				return false;
			}
			return true;
		} else {
			//If music is being played

			//If the music is paused
			if (Mix_PausedMusic() == 1) {
				//Resume the music
				Mix_ResumeMusic();
			} else {
				//If the music is playing

				//Pause the music
				Mix_PauseMusic();
			}
			return true;
		}
		return false;
	} catch (exception e) {
		Logger::getInstance()->log(Error, "Ocurrio un error al reproducir el sonido " + id);
		return false;
	}
}
bool ManejadorAudio::pauseTrack(std::string id) {
	try {
		Mix_Music* music = sonidos[id];
		if (Mix_PlayingMusic() > 0) {
			Mix_PauseMusic();
		}
	}
	catch (exception e) {
		Logger::getInstance()->log(Error, "Ocurrio un error al reproducir el sonido " + id);
		return false;
	}
}

bool ManejadorAudio::startTrack(std::string id) {
	try {
		Mix_Music* music = sonidos[id];

		//If there is no music playing
		if (Mix_PlayingMusic() == 0) {
			//Play the music
			Mix_PlayMusic(music, -1);
			return true;
		}
		else {
			//If the music is paused
			if (Mix_PausedMusic() == 1) {
				Mix_PlayMusic(music, -1);
				return true;
			}
			else {
				//Pause the music
				Mix_PauseMusic();
				Mix_PlayMusic(music, -1);
				return true;
			}
			return true;
		}
		return false;
	}
	catch (exception e) {
		Logger::getInstance()->log(Error, "Ocurrio un error al reproducir el sonido " + id);
		return false;
	}
}

bool ManejadorAudio::playTrackOnce(std::string id) {
	try {
		Mix_Music* music = sonidos[id];

		Mix_PlayMusic(music, 1);
		////If there is no music playing
		//if (Mix_PlayingMusic() == 0) {
		//	//Play the music
		//	Mix_PlayMusic(music, 1);
		//	return true;
		//}
	}
	catch (exception e) {
		Logger::getInstance()->log(Error, "Ocurrio un error al reproducir el sonido " + id);
		return false;
	}
}