/**
* @file input.h
*/
/* Receives the input events, does appropriate actions and forwards
 * mouse events to clickable objects at current mouse position.
 */

#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

#include <functional>
using namespace std;

#include "group.h"

class Clickable;

/**
 * @brief Zpracovává vstupy.
 */
class Input
{
private:
	friend class Engine;
	friend class Group;

	Input() {}

	static void update(Object * root);
	static void objectRemoved(Object * c);

	static Clickable * selected;

	static int mPosX;
	static int mPosY;
};

#endif // INPUT_H
