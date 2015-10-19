/**
* @file window.h
*/
/*
 * Window is a wrapper of SDL Window and SDL Renderer. It opens a window and
 * sets up the renderer, then in each frame renders the entire scene.
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

#include <string>
using namespace std;

/**
 * @brief Statická třída, kterou používá Engine pro vytvoření okna.
 */
class Window
{
public:
	/**
	 * @brief Vrátí SDL renderer, který je možno použít pro renderování vlastních objektů.
	 * @return
	 */
	static SDL_Renderer * getRenderer() { return sdlRenderer; }

	/**
	 * @brief Vrátí šířku okna.
	 * @return
	 */
	static int getWidth();

	/**
	 * @brief Vrátí výšku okna.
	 * @return
	 */
	static int getHeight();

private:
	friend class Engine;

	Window() {}

	static bool open(const string & title, int width, int height, bool fullscreen = false, int offsetX = 0, int offsetY = 0);
	static void close();

	static void clear();
	static void render();

	static SDL_Window * sdlWindow;
	static SDL_Renderer * sdlRenderer;
};

#endif // WINDOW_H
