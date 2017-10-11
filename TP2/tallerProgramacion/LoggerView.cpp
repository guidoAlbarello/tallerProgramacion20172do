#include "LoggerView.h"



LoggerView::LoggerView(SDL_Renderer* renderer)
{
	this->gRenderer = renderer;
	gPromptTextTexture = new Ltexture(gRenderer);
	gInputTextTexture = new Ltexture(gRenderer);
	gPromptPasswordTextTexture = new Ltexture(gRenderer);
	gInputPasswordTextTexture = new Ltexture(gRenderer);
}

LoggerView::~LoggerView()
{
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
	gFont = NULL;
}

bool LoggerView::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont("lazy.ttf", 28);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		//Render the prompt
		SDL_Color textColor = { 0, 0, 0, 0xFF };

		if (!gPromptTextTexture->loadFromRenderedText("Enter Text:", textColor))
		{
			printf("Failed to render prompt text!\n");
			success = false;
		}
	}

	return success;
}

void LoggerView::showLogin() {
	SDL_Color textColor = { 0, 0, 0, 0xFF };
	bool success = true;

	if (!gPromptTextTexture->loadFromRenderedText("Enter Username:", textColor))
	{
		printf("Failed to render prompt text!\n");
		success = false;
	}
	if (!gPromptPasswordTextTexture->loadFromRenderedText("Enter Password:", textColor))
	{
		printf("Failed to render prompt text!\n");
		success = false;
	}
	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	//Set text color as black

	//The current input text.
	std::string inputText = "User Name";
	gInputTextTexture->loadFromRenderedText(inputText.c_str(), textColor);


	//Enable text input
	SDL_StartTextInput();

	if (!loadMedia())
	{
		printf("Failed to load media!\n");
	} else 
	{
		//While application is running
		bool isWrittingPassord = false;
		while (!quit)
		{
			bool renderText = false;
			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{
				//Special key input
				if (e.type == SDL_KEYDOWN)
				{
					if (e.key.keysym.sym == SDLK_ESCAPE)
					{
						quit = true;
					}
					if (e.key.keysym.sym == SDLK_RETURN)
					{
						if (isWrittingPassord == false) {
							isWrittingPassord = true;
							inputText = "";
						}
						else {
							//procesar user y pass
						}
					}
					//Handle backspace
					if (e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0)
					{
						//lop off character
						inputText.pop_back();
						renderText = true;
					}
				}
				//Special text input event
				else if (e.type == SDL_TEXTINPUT)
				{
					//Append character
					inputText += e.text.text;
					renderText = true;
				}
			}

			//Rerender text if needed
			if (renderText)
			{
				//Text is not empty
				if (inputText != "")
				{
					if (!isWrittingPassord) {
						//Render new text
						gInputTextTexture->loadFromRenderedText(inputText.c_str(), textColor);
					}
					else {
						gInputPasswordTextTexture->loadFromRenderedText(inputText.c_str(), textColor);
					}
				}
				//Text is empty
				else
				{
					if (!isWrittingPassord) {
						//Render space texture
						gInputTextTexture->loadFromRenderedText(" ", textColor);
					}
					else {
						gInputPasswordTextTexture->loadFromRenderedText(" ", textColor);
					}
				}
			}

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
	}

	//Disable text input
	SDL_StopTextInput();

}