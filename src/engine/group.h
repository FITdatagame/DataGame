/**
* @file group.h
*/
/* A scene graph component which can act as a parent of other nodes.
 * When the group receives Update, Render or setTransform call it
 * forwards this to the child objects.
 */

#ifndef GROUP_H
#define GROUP_H

#include "object.h"

#include <vector>
#include <functional>
using namespace std;

/**
 * @brief Reprezentuje vnitří uzly v grafu scény. Je to objekt, který může obsahovat další objekty (potomky).
 */
class Group : public Object
{
public:
	Group(const string & name = "") : Object(name) {}

	/**
	 * @brief Přidání potomka.
	 * @param child
	 */
	void addChild(Object * child);

	/**
	 * @brief Vrátí potomka podle jména.
	 * @param name
	 * @return
	 */
	Object * getChild(const string & name);

	/**
	 * @brief Projde všechny potomky a zavolá na ně předanou funkci.
	 * @param func
	 */
	void iterateChildren(function<void (Object * child)> func);

	/**
	 * @brief Odstraní potomka podle jména a případně ho smaže.
	 * @param name
	 * @param deleteChild
	 * @return
	 */
	bool removeChild(const string & name, bool deleteChild = false);

	/**
	 * @brief Odstraní potomka.
	 * @param child
	 * @return
	 */
	bool removeChild(Object * child);

	/**
	 * @brief Odstraní a případně smaže všechny potomky.
	 * @param deleteChildren
	 */
	void removeChildren(bool deleteChildren = false);

	virtual void setVisible(bool v);
	virtual void setEnabled(bool e);

	virtual void setParentPosition(const Vec2 & pos);
	virtual void setPosition(const Vec2 & pos);

	virtual bool input(function<bool (Clickable * o)> fn);
	virtual void update();
	virtual void render();

protected:
	vector<Object*> children;

private:
	void refreshChildrenPositions(const Vec2 & pos);
};

#endif // GROUP_H
