/**
* @file object.h
*/
/* Object is a basic node which can be added to the scene graph.
 * It has a name, a position and a parent's position which is
 * used to calculate the absolute position of the object.
 */

#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <functional>
using namespace std;

#include "vec2.h"

class Clickable;

/**
 * @brief Základní třída reprezentující jakýkoliv uzel v grafu scény. Každý objekt má 2D pozici. Každý potomek ve stromu dědí pozici svého rodiče.
 */
class Object
{
public:
	Object(const string & name = "") : name(name) {}
	virtual ~Object() {}

	string getName() const { return name; }
	void setName(const string & name) { this->name = name; }

	virtual void setVisible(bool v) { visible = v; }
	bool getVisible() const { return visible; }

	virtual void setEnabled(bool e) { enabled = e; }
	bool getEnabled() const { return enabled; }

	virtual void setPosition(const Vec2 & pos) { position = pos; }
	virtual void setParentPosition(const Vec2 & pos) { parentPos = pos; }
	Vec2 getPosition() const { return position; }
	Vec2 getCombinedPosition() const { return parentPos + position; }

	/**
	 * @brief Funkce rekurzivně volaná na celý graf scény, pokud nastane vstup.
	 * @param fn
	 * @return
	 */
	virtual bool input(function<bool (Clickable * o)> fn) { return false; }

	/**
	 * @brief Funkce rekurzivně volaná na celý graf scény každý frame před renderem.
	 */
	virtual void update() {}

	/**
	 * @brief Funkce rekurzivně volaná na celý graf scény. Vykresluje jednotlivé položky.
	 */
	virtual void render() {}

protected:
	string name;
	bool visible = true;
	bool enabled = true;

	Vec2 position;
	Vec2 parentPos;
};

#endif // OBJECT_H
