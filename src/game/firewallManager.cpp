#include "firewallManager.h"

#include <cstdio>

#include "window.h"

FirewallManager::FirewallManager()
{
	elements = new Group();
	portsGroup = new Group();

	Image * background = new Image("res/firewall.png");
	elements->addChild(background);

	Image * close = new Image("res/close.png");
	elements->addChild(close);
	close->setPosition(Vec2(background->getWidth() - close->getWidth() - 30, 30));

	close->setOnClickFunc(
		[this](int x, int y, bool right)
		{
			hide();
			return true;
		}
	);

	infoText = new Text("Kliknutím na port ho zavřeš nebo otevřeš.", 1.5, Color(0, 0, 0));
	elements->addChild(infoText);
	infoText->setPosition(Vec2((background->getWidth() - infoText->getWidth()) / 2, 30));

	setPosition(Vec2((Window::getWidth() - background->getWidth()) / 2, (Window::getHeight() - background->getHeight()) / 2));
}

FirewallManager::~FirewallManager()
{
	elements->removeChildren(true);
	portsGroup->removeChildren(true);

	delete elements;
	delete portsGroup;
}

void FirewallManager::refresh()
{
	portsGroup->removeChildren(true);

	int pos = 0;

	for(auto & p : ports)
	{
		char buf[64];
		sprintf(buf, "%d (%s)", p.second.number, p.second.name.c_str());

		Image * img = new Image(p.second.open ? "res/openedPort.png" : "res/closedPort.png");
		Text * txt = new Text(buf);

		int port = p.first;
		img->setOnClickFunc(
			[this, port](int x, int y, bool right)
			{
				ports[port].open = !ports[port].open;
				refresh();
				return true;
			}
		);

		img->setPosition(Vec2((pos % 5) * 150 + 80, (pos / 5) * 100 + 100));
		txt->setPosition(Vec2((pos % 5) * 150 + 80 - (txt->getWidth() - img->getWidth()) / 2, (pos / 5) * 100 + 100 + 32));

		portsGroup->addChild(img);
		portsGroup->addChild(txt);

		pos++;
	}
}

Port FirewallManager::getRandomClosedPort()
{
	vector<Port*> closedPorts;

	for(auto & p : ports)
		if(!p.second.open)
			closedPorts.push_back(&p.second);

	int portCnt = closedPorts.size();
	int pos = Engine::random(0, portCnt - 1);

	if(portCnt > 0) return *closedPorts[pos];
	else return Port();
}

Port FirewallManager::getRandomOpenPort()
{
	vector<Port*> openPorts;

	for(auto & p : ports)
		if(p.second.open)
			openPorts.push_back(&p.second);

	int portCnt = openPorts.size();
	int pos = Engine::random(0, portCnt - 1);

	if(portCnt > 0) return *openPorts[pos];
	else return Port();
}

void FirewallManager::show()
{
	addChild(elements);
	addChild(portsGroup);
	Engine::setInputRoot(this);
}

void FirewallManager::hide()
{
	removeChild(elements);
	removeChild(portsGroup);
	Engine::setInputRoot(NULL);
}
