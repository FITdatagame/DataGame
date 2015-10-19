/**
* @file engine.h
*/
/* Main part which manages all engine components. Creates the window, manages input handlers,
 * containes the scene root, handles the rendering, measures length of frames and destroys
 * everything once finished.
 */

#ifndef ENGINE_H
#define ENGINE_H

#include <string>
using namespace std;

#include "input.h"
#include "group.h"

/**
 * @brief Hlavní statická třída zodpovědná za inicializaci SDL, volání updatů a renderů a uvolnění prostředků.
 */
class Engine
{
public:
	/**
	 * @brief Inicializace enginu.
	 * @param title
	 * @param width
	 * @param height
	 * @param fullscreen
	 * @param offsetX
	 * @param offsetY
	 * @return
	 */
	static bool initialize(const string & title, int width, int height, bool fullscreen = false,
						   int offsetX = 0, int offsetY = 0);
	/**
	 * @brief Deinicializace enginu.
	 */
	static void deinitialize();

	/**
	 * @brief Spuštění enginu.
	 */
	static void run();

	/**
	 * @brief Zastavení enginu.
	 */
	static void stop();

	/**
	 * @brief Přidání objektu do kořenového uzlu.
	 * @param obj
	 */
	static void addObject(Object * obj) { root->addChild(obj); }
	static void setInputRoot(Object * obj) { cout << "set input " << (long int)obj << endl; inputRoot = obj; }

	/**
	 * @brief Vrátí dobu trvání posledního framu.
	 * @return
	 */
	static double getDelta() { return timeDelta; }

	/**
	 * @brief Vrátí náhodné číslo mezi 2 mezemi.
	 * @param min
	 * @param max
	 * @return
	 */
	static int random(int min, int max);

private:
	Engine() {}

	static bool initialized;
	static bool isRunning;

	static Group * root;
	static Object * inputRoot;

	static double timeDelta;
};

#endif // ENGINE_H
