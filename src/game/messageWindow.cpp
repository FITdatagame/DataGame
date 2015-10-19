#include "messageWindow.h"

#include "text.h"
#include "rectangle.h"
#include "window.h"

MessageWindow::MessageWindow()
{
	elements = new Group();
	elements->setPosition(Vec2(Window::getWidth() / 4, Window::getHeight() / 4));

	clear();
}

MessageWindow::~MessageWindow()
{
	elements->removeChildren(true);
	delete elements;
}

void MessageWindow::clear()
{
	elements->removeChildren(true);
	optPos = 0;

	Rectangle * background = new Rectangle(Window::getWidth() / 2, Window::getHeight() / 2, Color(50, 50, 50, 255));
	elements->addChild(background);
}

void MessageWindow::setMessage(const string & message)
{
	clear();

	Text * msg = new Text(message, 1.0, Color(255, 255, 255), Window::getWidth() / 2 - 20);
	msg->setPosition(Vec2(10, 10));
	elements->addChild(msg);
	optPos += msg->getHeight() + 15;
}

void MessageWindow::addOption(const string & text, int code)
{
	Text * opt = new Text(text, 1.0, Color(200, 200, 200), Window::getWidth() / 2 - 30);
	elements->addChild(opt);
	opt->setPosition(Vec2(20, optPos));
	optPos += opt->getHeight();

	opt->setOnClickFunc(
		[this, code](int x, int y, bool right)
		{
			if(onClickFunc)
				onClickFunc(code);

			return true;
		}
	);

	opt->setOnInFunc(
		[this, opt](int x, int y)
		{
			opt->setColor(Color(240, 240, 240));

			return true;
		}
	);

	opt->setOnOutFunc(
		[this, opt](int x, int y)
		{
			opt->setColor(Color(200, 200, 200));

			return true;
		}
	);
}

void MessageWindow::show()
{
	addChild(elements);
	Engine::setInputRoot(this);
}

void MessageWindow::hide()
{
	removeChild(elements);
	Engine::setInputRoot(NULL);
}
