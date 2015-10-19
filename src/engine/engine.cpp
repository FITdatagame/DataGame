#include "engine.h"

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <ctime>
using namespace std;

#include "window.h"
#include "image.h"

bool Engine::initialized = false;
bool Engine::isRunning = false;
Group * Engine::root = NULL;
Object * Engine::inputRoot = NULL;
double Engine::timeDelta;

// initialize SDL, open window and create scene root
bool Engine::initialize(const string & title, int width, int height, bool fullscreen, int offsetX, int offsetY)
{
	srand(time(NULL));

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
		return false;
	}

	if(TTF_Init() != 0)
	{
		cout << "SDL_ttf Error: " << TTF_GetError() << endl;
		SDL_Quit();
		return false;
	}

	if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0)
	{
		cout << "SDL_image Error: " << IMG_GetError() << endl;
		SDL_Quit();
		return false;
	}
	
	SDL_DisplayMode dispMode;
	SDL_GetCurrentDisplayMode(0, &dispMode);
	if(width == 0) width = dispMode.w;
	if(height == 0) height = dispMode.h;

	if(!Window::open(title, width, height, fullscreen, offsetX, offsetY))
	{
		SDL_Quit();
		return false;
	}

	SDL_StartTextInput();

	root = new Group("Scene root");

	return initialized = true;
}

// destroy everything
void Engine::deinitialize()
{
	if(!initialized) return;

	delete root;
	Image::freeCache();

	SDL_Quit();
	initialized = false;
}

// the main loop
void Engine::run()
{
	if(!initialized) return;

	double oldTime = SDL_GetTicks() / 1000.0;
	double totalTime = 0;
	int ticks = 0;

	isRunning = true;

	while(isRunning)
	{
		// call updates and render the scene
		Input::update(inputRoot ? inputRoot : root);
		root->update();

		Window::clear();
		root->render();
		Window::render();

		// measure how long it took
		double newTime = SDL_GetTicks() / 1000.0;
		timeDelta = newTime - oldTime;
		oldTime = newTime;

		ticks++;
		totalTime += timeDelta;

		if(totalTime > 3.0)
		{
			double fps = ticks / totalTime;
			totalTime = 0;
			ticks = 0;

			cout << "FPS: " << fps << " " << timeDelta << endl;
		}
	}
}

void Engine::stop()
{
	isRunning = false;
}

int Engine::random(int min, int max)
{
	double r = rand() * 1.0 / RAND_MAX;
	return min + round(r * (max - min));
}
