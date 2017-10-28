#include "SpriteFactory.h"

SpriteFactory* SpriteFactory::instance = 0;

SpriteFactory::SpriteFactory()
{
}


SpriteFactory::~SpriteFactory()
{
}

SpriteFactory* SpriteFactory::getInstance() {
	if (!instance) {
		instance = new SpriteFactory();
	}

	return instance;
}

Sprite* SpriteFactory::crearSprite(int imageId) {
	Sprite* sprite = new Sprite();
	Vector* posicion = new Vector();
	//sprite->id = "main";
	switch (imageId) {
		case carro1:
			posicion->setX(1085);
			posicion->setY(480);
			//sprite->setValues(80, 41, posicion, 1);
			break;
		case carro2:
			posicion->setX(1383);
			posicion->setY(825);
			//sprite->setValues(80, 59, posicion, 1);
			break;
		case palmera:
			posicion->setX(5);
			posicion->setY(5);
			//sprite->setValues(215, 540, posicion, 1);
			break;		
		case cactus:
			posicion->setX(929);
			posicion->setY(897);
			//sprite->setValues(235, 118, posicion, 1);
			break;
		default:
			posicion->setX(20);
			posicion->setY(20);
			//sprite->setValues(20, 20, posicion, 1);
			break;
	}
	return sprite;
}
