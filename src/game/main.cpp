/**
* @mainpage Údržba a ochrana dat (didaktická strategická hra)
* Více informací na https://trac.project.fit.cvut.cz/DataGame/wiki
*/

#include <iostream>
using namespace std;

#include "engine.h"
#include "game.h"
#include "tests.h"

/* Initialize engine, create window, create and add main game object,
 * run the engine and free everything at the end.
 */
int main(int argc, char * argv[])
{
	if(argc > 1 && strcmp(argv[1], "testing") == 0)
		runTesting();

	Engine::initialize("DataGame", 1280, 720, false, 20, 20);

	Game * g = new Game();
	Engine::addObject(g);

	Engine::run();

	delete g;
	Engine::deinitialize();

	return 0;
}
