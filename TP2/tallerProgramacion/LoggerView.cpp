#include "LoggerView.h"

LoggerView::LoggerView(SDL_Renderer* renderer) {
	this->gRenderer = renderer;
	gPromptTextTexture = new Ltexture(gRenderer);
	gInputTextTexture = new Ltexture(gRenderer);
	gPromptPasswordTextTexture = new Ltexture(gRenderer);
	gInputPasswordTextTexture = new Ltexture(gRenderer);
	gBackgroundImage = new Ltexture(gRenderer);
	this->mensajeLogin = "Usuario:";
}

LoggerView::~LoggerView() {
	if (gPromptTextTexture != NULL) {
		gPromptTextTexture->free();
	}
	if (gInputTextTexture != NULL) {
		gInputTextTexture->free();
	}
	if (gPromptPasswordTextTexture != NULL) {
		gPromptPasswordTextTexture->free();
	}
	if (gInputPasswordTextTexture != NULL) {
		gInputPasswordTextTexture->free();
	}
	if (gBackgroundImage != NULL) {
		gBackgroundImage->free();
	}
	
	TTF_CloseFont(gFont);
	Mix_HaltMusic(); //Stop the music
	delete usuario;
	gFont = NULL;
}
void LoggerView::setMensajeLogin(std::string mensaje) {
	this->mensajeLogin = mensaje;

	if (!gPromptTextTexture->loadFromRenderedText(this->mensajeLogin, textColor)) {
		Logger::getInstance()->log(Error, "Ocurrio un error al iniciar la textura Usuario:");
		//success = false;
	}
}

bool LoggerView::init() {
	textColor = { 0, 0, 0, 0xFF };
	success = true;
	usuario = new Usuario();
	datosCargados = false;
	if (!gPromptTextTexture->loadFromRenderedText(this->mensajeLogin, textColor)) {
		Logger::getInstance()->log(Error, "Ocurrio un error al iniciar la textura Usuario:");
		success = false;
	}
	if (!gPromptPasswordTextTexture->loadFromRenderedText("Contrasenia:", textColor)) {
		Logger::getInstance()->log(Error, "Ocurrio un error al iniciar la textura Contraseña:");
		success = false;
	}
	if (!gBackgroundImage->loadFromFile("imagenes/init_background.bmp")) {
		Logger::getInstance()->log(Error, "Ocurrio un error al iniciar la imagen de fondo de logueo");
		success = false;
	}

	this->initSonido();
	ManejadorAudio::getInstance()->startOrPauseTrack("initTrack");

	inputText = "";
	//aca estaba el error decia su suario y se sumaba al otro
	gInputTextTexture->loadFromRenderedText(inputText.c_str(), textColor);
	isWrittingPassord = false;
	this->terminoIniciar = true;
	return false;
}

void LoggerView::render() {
	//if (this->terminoIniciar) {
	//	//Clear screen
	//	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	//	SDL_RenderClear(gRenderer);

	//	// Render imagen fondo
	//	SDL_Rect* rectanguloFullscreen = new SDL_Rect();
	//	rectanguloFullscreen->w = 800;
	//	rectanguloFullscreen->h = 600;
	//	gBackgroundImage->render(0, 0, rectanguloFullscreen);


	//	//Render text textures
	//	int h1 = gPromptTextTexture->getHeight() + gInputTextTexture->getHeight();
	//	int h2 = h1 + gPromptPasswordTextTexture->getHeight();
	//	gPromptTextTexture->render(150 + (SCREEN_WIDTH - gPromptTextTexture->getWidth()) / 2, 280);
	//	gInputTextTexture->render(150 + (SCREEN_WIDTH - gInputTextTexture->getWidth()) / 2, 310);
	//	gPromptPasswordTextTexture->render(150 + (SCREEN_WIDTH - gPromptPasswordTextTexture->getWidth()) / 2, 360);
	//	gInputPasswordTextTexture->render(150 + (SCREEN_WIDTH - gInputPasswordTextTexture->getWidth()) / 2, 390);

	//	//Update screen
	//	SDL_RenderPresent(gRenderer);
	//}
}

void LoggerView::update() {
	bool quit = false, renderText = false;
	//Event handler
	SDL_Event e;

	//Enable text input
	SDL_StartTextInput();

	//Handle events on queue
	while (SDL_PollEvent(&e) != 0) {
		//Special key input
		if (e.type == SDL_KEYDOWN) {
			if (e.key.keysym.sym == SDLK_ESCAPE) {
				ManejadorInput::getInstance()->setCerrar(true);
			}
			if (e.key.keysym.sym == SDLK_RETURN) {
				if (inputText != "") {
					if (isWrittingPassord == false) {
						isWrittingPassord = true;
						usuario->setNombre(inputText);
						inputText = "";
					}
					else {
						//procesar user y pass
						if (inputText != "") {
							quit = true;
							datosCargados = true;
							renderText = false;
							usuario->setPassword(inputText);
							break;
						}
					}
				}
			}

			if (e.type == SDL_WINDOWEVENT) {
				if (e.window.event == SDL_WINDOWEVENT_CLOSE)
					ManejadorInput::getInstance()->setCerrar(true);
			}
			if (e.type == SDL_QUIT) {
				ManejadorInput::getInstance()->setCerrar(true);
			}
			//Handle backspace
			if (e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0) {
				//lop off character
				inputText.pop_back();
				renderText = true;
			}
		}
		//Special text input event
		else if (e.type == SDL_TEXTINPUT) {
			//Append character
			inputText += e.text.text;
			renderText = true;
		}

		if (e.type == SDL_WINDOWEVENT) {
			if (e.window.event == SDL_WINDOWEVENT_CLOSE)
				ManejadorInput::getInstance()->setCerrar(true);
		}
		if (e.type == SDL_QUIT) {
			ManejadorInput::getInstance()->setCerrar(true);
		}
	}

	//Rerender text if needed
	if (renderText) {
		//Text is not empty
		if (inputText != "") {
			if (!isWrittingPassord) { 
				//Render new text
				gInputTextTexture->loadFromRenderedText(inputText.c_str(), textColor);
			} else {
				gInputPasswordTextTexture->loadFromRenderedText(inputText.c_str(), textColor);
			}
		}
		//Text is empty
		else {
			if (!isWrittingPassord) {
				//Render space texture
				gInputTextTexture->loadFromRenderedText(" ", textColor);
			} else {
				gInputPasswordTextTexture->loadFromRenderedText(" ", textColor);
			}
		}
	}
	SDL_StopTextInput();
	if (this->terminoIniciar) {
		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		// Render imagen fondo
		SDL_Rect* rectanguloFullscreen = new SDL_Rect();
		rectanguloFullscreen->w = 800;
		rectanguloFullscreen->h = 600;
		gBackgroundImage->render(0, 0, rectanguloFullscreen);


		//Render text textures
		int h1 = gPromptTextTexture->getHeight() + gInputTextTexture->getHeight();
		int h2 = h1 + gPromptPasswordTextTexture->getHeight();
		gPromptTextTexture->render(150 + (SCREEN_WIDTH - gPromptTextTexture->getWidth()) / 2, 280);
		gInputTextTexture->render(150 + (SCREEN_WIDTH - gInputTextTexture->getWidth()) / 2, 310);
		gPromptPasswordTextTexture->render(150 + (SCREEN_WIDTH - gPromptPasswordTextTexture->getWidth()) / 2, 360);
		gInputPasswordTextTexture->render(150 + (SCREEN_WIDTH - gInputPasswordTextTexture->getWidth()) / 2, 390);

		//Update screen
		SDL_RenderPresent(gRenderer);
	}
	
}

Usuario * LoggerView::getUsuario() {
	if (datosCargados)
		return this->usuario;
	else
		return NULL;
}

bool LoggerView::close() {
	return false;
}

Usuario* LoggerView::showLogin() {
	//Main loop flag
	bool quit = false;

	//Event handler


	//Set text color as black

	return usuario;

}

void LoggerView::initSonido() {
	ManejadorAudio::getInstance()->load("sound/init_track.wav", "initTrack");
}