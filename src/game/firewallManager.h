/**
* @file firewallManager.h
*/

#ifndef FIREWALLMANAGER_H
#define FIREWALLMANAGER_H

#include <map>
using namespace std;

#include "group.h"
#include "image.h"
#include "text.h"

/**
* @struct Port
* @brief Představuje jeden síťový port ve firewallu.
*/
struct Port {
	Port(int number = 0, const string & name = "") : number(number), name(name) {}

	int number;
	string name;
	bool open = false;
};

/**
* @class FirewallManager
* @brief Zobrazuje porty ve firewallu a umožňuje jejich otevírání a zavírání.
*/
class FirewallManager : public Group
{
public:
	FirewallManager();
	~FirewallManager();

	/** @brief Vrátí port podle čísla.
	*/
	Port getPort(int p) { return ports[p]; }

	/** @brief Vybere náhodně jeden zavřený port.
	*/
	Port getRandomClosedPort();

	/** @brief Vybere náhodně jeden otevřený port.
	*/
	Port getRandomOpenPort();

	/** @brief Přidá nový port do seznamu.
	*/
	void addPort(const Port & p) { ports[p.number] = p; refresh(); }

	/** @brief Odebere port ze seznamu.
	*/
	void removePort(int p) { ports.erase(p); refresh(); }

	/** @brief Otevře zadaný port.
	*/
	void openPort(int p) { ports[p].open = true; refresh(); }

	/** @brief Zavře zadaný port.
	*/
	void closePort(int p) { ports[p].open = false; refresh(); }

	/** @brief Zobrazí okno s firewallem.
	*/
	void show();

	/** @brief Skryje okno s firewallem.
	*/
	void hide();

private:
	/** @brief Překreslí okno.
	*/
	void refresh();

	map<int, Port> ports;
	Group * portsGroup;
	Group * elements;
	Image * closeButton;
	Text * infoText;
};

#endif // FIREWALLMANAGER_H
