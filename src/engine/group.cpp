#include "group.h"

#include "input.h"
#include "engine.h"

bool Group::input(function<bool (Clickable * o)> fn)
{
	if(!enabled) return false;

	for(auto it = children.rbegin(); it != children.rend(); it++)
		if((*it)->input(fn))
			return true;

	return false;
}

// forward the call to the children
void Group::update()
{
	if(!enabled) return;

	for(auto & c : children)
		c->update();
}

// forward the call to the children
void Group::render()
{
	if(!visible) return;

	for(auto & c : children)
		c->render();
}

void Group::addChild(Object * child)
{
	for(auto & c : children)
		if(c == child)
			return;

	children.push_back(child);

	child->setParentPosition(getCombinedPosition());
}

Object * Group::getChild(const string & name)
{
	for(auto & c : children)
		if(c->getName() == name)
			return c;
	return NULL;
}

void Group::iterateChildren(function<void (Object * child)> func)
{
	for(auto & c : children)
		func(c);
}

// when removing child by name we can opt to delete it as well
bool Group::removeChild(const string & name, bool deleteChild)
{
	for(auto it = children.begin(); it != children.end(); it++)
		if((*it)->getName() == name)
		{
			if(deleteChild) delete *it;
			Input::objectRemoved(*it);
			children.erase(it);

			return true;
		}

	return false;
}

bool Group::removeChild(Object * child)
{
	for(auto it = children.begin(); it != children.end(); it++)
		if(*it == child)
		{
			Input::objectRemoved(*it);
			children.erase(it);

			return true;
		}

	return false;
}

void Group::removeChildren(bool deleteChildren)
{
	for(auto & c : children)
	{
		Input::objectRemoved(c);
		if(deleteChildren) delete c;
	}

	children.clear();
}

void Group::setVisible(bool v)
{
	visible = v;

	for(auto & c : children)
		c->setVisible(v);
}

void Group::setEnabled(bool e)
{
	enabled = e;

	for(auto & c : children)
		c->setEnabled(e);
}

/* When the position is changed we must push the change down
 * the scene graph so that positions in the children are updated.
 */

void Group::setParentPosition(const Vec2 & pos)
{
	Object::setParentPosition(pos);
	refreshChildrenPositions(getCombinedPosition());
}

void Group::setPosition(const Vec2 & pos)
{
	Object::setPosition(pos);
	refreshChildrenPositions(getCombinedPosition());
}

void Group::refreshChildrenPositions(const Vec2 & pos)
{
	for(auto & c : children)
		c->setParentPosition(pos);
}
