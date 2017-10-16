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

	return false;
}

void LoggerView::render() {
	//The current input text.
	std::string inputText = "User Name";
	gInputTextTexture->loadFromRenderedText(inputText.c_str(), textColor);


	//Enable text input

	bool isWrittingPassord = false;
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

	//Disable text input
	SDL_StopTextInput();
}

void LoggerView::update() {
	bool quit = false, renderText = false;
	SDL_Event e = ManejadorInput::getInstance()->getEntradaTexto(inputText);
	if (e.type == SDL_KEYDOWN) {
		if (e.key.keysym.sym == SDLK_ESCAPE) {
			quit = true;		//cambiar
		}
		if (e.key.keysym.sym == SDLK_RETURN) {
			if (isWrittingPassword == false) {
				isWrittingPassword = true;
				usuario->setNombre(inputText);
				inputText = "";
			} else {
				//procesar user y pass
				quit = true;
				renderText = false;
				usuario->setPassword(inputText);
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

	//Rerender text if needed
	if (renderText) {
		//Text is not empty
		if (inputText != "") {
			if (!isWrittingPassword) {
				//Render new text
				gInputTextTexture->loadFromRenderedText(inputText.c_str(), textColor);
			} else {
				gInputPasswordTextTexture->loadFromRenderedText(inputText.c_str(), textColor);
			}
		}
		//Text is empty
		else {
			if (!isWrittingPassword) {
				//Render space texture
				gInputTextTexture->loadFromRenderedText(" ", textColor);
			} else {
				gInputPasswordTextTexture->loadFromRenderedText(" ", textColor);
			}
		}
	}
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