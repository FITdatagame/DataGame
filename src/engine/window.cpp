#include "window.h"

#include <iostream>
using namespace std;

#include "engine.h"

SDL_Window * Window::sdlWindow = NULL;
SDL_Renderer * Window::sdlRenderer = NULL;

/* Opens a new window with specified parameters and sets up the renderer. */
bool Window::open(const string &title, int width, int height, bool fullscreen, int offsetX, int offsetY)
{
	sdlWindow = SDL_CreateWindow(title.c_str(),
								 offsetX,
								 offsetY,
								 width,
								 height,
								 fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);

	if(!sdlWindow)
	{
		cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
		return false;
	}

	// -1 = choose rendering device automatically; SDL_RENDERER_PRESENTVSYNC can be used to enable vsync
	sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if(!sdlRenderer)
	{
		SDL_DestroyWindow(sdlWindow);
		cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
		return false;
	}

	SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
	SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	// if in fullscreen mode scale this to the screen size
	SDL_RenderSetLogicalSize(sdlRenderer, width, height);

	return true;
}

void Window::close()
{
	if(sdlRenderer) SDL_DestroyRenderer(sdlRenderer);
	if(sdlWindow) SDL_DestroyWindow(sdlWindow);
}

void Window::clear()
{
	if(!sdlRenderer) return;

	SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
	SDL_RenderClear(sdlRenderer);
}

void Window::render()
{
	if(!sdlRenderer) return;

	SDL_RenderPresent(sdlRenderer);
}

int Window::getWidth()
{
	if(!sdlWindow) return 0;

	int x, y;
	SDL_GetWindowSize(sdlWindow, &x, &y);

	return x;
}

int Window::getHeight()
{
	if(!sdlWindow) return 0;

	int x, y;
	SDL_GetWindowSize(sdlWindow, &x, &y);

	return y;
}
