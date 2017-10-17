#include "LoggerView.h"

LoggerView::LoggerView(SDL_Renderer* renderer) {
	this->gRenderer = renderer;
	gPromptTextTexture = new Ltexture(gRenderer);
	gInputTextTexture = new Ltexture(gRenderer);
	gPromptPasswordTextTexture = new Ltexture(gRenderer);
	gInputPasswordTextTexture = new Ltexture(gRenderer);
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
	TTF_CloseFont(gFont);
	delete usuario;
	gFont = NULL;
}

bool LoggerView::init() {
	textColor = { 0, 0, 0, 0xFF };
	success = true;
	usuario = new Usuario();
	datosCargados = false;
	if (!gPromptTextTexture->loadFromRenderedText("Enter Username:", textColor)) {
		//printf("Failed to render prompt text!\n");
		success = false;
	}
	if (!gPromptPasswordTextTexture->loadFromRenderedText("Enter Password:", textColor))
		//if (!gPromptPasswordTextTexture->loadFromFile("fondo/sky.png"))
	{
		//printf("Failed to render prompt text!\n");
		success = false;
	}
	//The current input text.
	inputText = "User Name";
	gInputTextTexture->loadFromRenderedText(inputText.c_str(), textColor);
	isWrittingPassord = false;
	return false;
}

void LoggerView::render() {
	//Clear screen
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	//Render text textures
	int h1 = gPromptTextTexture->getHeight() + gInputTextTexture->getHeight();
	int h2 = h1 + gPromptPasswordTextTexture->getHeight();
	gPromptTextTexture->render((SCREEN_WIDTH - gPromptTextTexture->getWidth()) / 2, 0);
	gInputTextTexture->render((SCREEN_WIDTH - gInputTextTexture->getWidth()) / 2, gPromptTextTexture->getHeight());
	gPromptPasswordTextTexture->render((SCREEN_WIDTH - gPromptPasswordTextTexture->getWidth()) / 2, h1);
	gInputPasswordTextTexture->render((SCREEN_WIDTH - gInputPasswordTextTexture->getWidth()) / 2, h2);

	//Update screen
	SDL_RenderPresent(gRenderer);


}

void LoggerView::update() {
	bool quit = false, renderText = false;
	//Event handler
	//Event handler
	SDL_Event e;

	//Set text color as black

	


	//Enable text input
	SDL_StartTextInput();

	

	//Handle events on queue
	while (SDL_PollEvent(&e) != 0) {
		//Special key input
		if (e.type == SDL_KEYDOWN) {
			if (e.key.keysym.sym == SDLK_ESCAPE) {
				quit = true;
			}
			if (e.key.keysym.sym == SDLK_RETURN) {
				if (isWrittingPassord == false) {
					isWrittingPassord = true;
					usuario->setNombre(inputText);
					inputText = "";
				} else {
					//procesar user y pass
					quit = true;
					datosCargados = true;
					renderText = false;
					usuario->setPassword(inputText);
					break;
				}
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