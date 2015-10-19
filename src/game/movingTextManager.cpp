#include "movingTextManager.h"

#include "window.h"

MovingTextManager::MovingTextManager()
{
	startingPos = Window::getWidth();
	speed = 150;
}

MovingTextManager::~MovingTextManager()
{
	removeChildren(true);
}

void MovingTextManager::addText(const string & text, int repeatCnt, const Color & color)
{
	if(repeatCnt <= 0) repeatCnt = -1;
	else repeatCnt--;

	MovingText t;
	t.repeatCnt = repeatCnt;
	t.text = new Text(text, 1.0, color);
	t.text->setPosition(Vec2(startingPos, 0));
	addChild(t.text);

	texts.push_back(t);

	startingPos += t.text->getWidth() + 20;
}

void MovingTextManager::clear()
{
	texts.clear();
	removeChildren(true);
}

void MovingTextManager::update()
{
	startingPos = Window::getWidth();

	auto it = texts.begin();

	while(it != texts.end())
	{
		Text * t = it->text;
		t->setPosition(t->getPosition() - Vec2(speed * Engine::getDelta(), 0));

		if(t->getPosition().getX() < -t->getWidth()
		&& !(it->repeatCnt < 0 || --it->repeatCnt >= 0))
		{
			removeChild(t);
			delete t;
			it = texts.erase(it);
			continue;
		}
		else ++it;

		if(t->getPosition().getX() + t->getWidth() + 20 > startingPos)
			startingPos = t->getPosition().getX() + t->getWidth() + 20;
	}

	for(auto t : texts)
		if(t.text->getPosition().getX() < -t.text->getWidth()
		&& (t.repeatCnt < 0 || t.repeatCnt >= 0))
		{
			t.text->setPosition(Vec2(startingPos, 0));
			startingPos += t.text->getWidth();
		}

	Group::update();
}
