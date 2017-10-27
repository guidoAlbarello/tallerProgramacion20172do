#pragma once
#ifndef SPRITE_FACTORY_H
#define SPRITE_FACTORY_H
#include "Sprite.h"

const int arbol = 1;
const int palmera = 2;
const int carro1 = 3;
const int carro2 = 4;
const int camion = 5;
const int cartel = 6;
const int cactus = 7;


class SpriteFactory{
public:
	static SpriteFactory* getInstance();
	Sprite* crearSprite(int spriteNumber);
private:
	static SpriteFactory* instance;
	SpriteFactory();
	~SpriteFactory();
};

#endif