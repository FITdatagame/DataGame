/**
* @file movingTextManager.h
*/

#ifndef MOVINGTEXTMANAGER_H
#define MOVINGTEXTMANAGER_H

#include "group.h"
#include "text.h"

/**
 * @class MovingTextManager
 * @brief Třída obsluhuje běžící text, který je v dolní části obrazovky.
 */
class MovingTextManager : public Group
{
public:
	MovingTextManager();
	~MovingTextManager();


	/** @brief Přidá nový text do fronty. Parametry určují, kolikrát má text běžet dokola (0 je napořád) a jakou má mít barvu.
	*/
	void addText(const string & text, int repeatCnt = 1, const Color & color = Color(255, 255, 255, 255));

	/** @brief Nastaví rychlost textu.
	*/
	void setSpeed(double sp) { speed = sp; }

	/** @brief Smaže všechny texty.
	*/
	void clear();


	/** @brief Pravidelně volaná funkce v každém framu.
	*/
	virtual void update();

private:

	/**
	* @struct MovingText
	* @brief Představuje jednu pohyblivou zprávu.
	*/
	struct  MovingText{
		Text * text;
		int repeatCnt;
	};


	/** @brief Pole zobrazovaných běžících textů.
	*/
	vector<MovingText> texts;


	/** @brief Pozice, na kterou se bude vkládat nový běžící text. Její zvyšování zajistí vkládání textů za sebe do fronty.
	*/
	int startingPos;

	/** @brief Rychlost běžícího textu.
	*/
	double speed;
};

#endif // MOVINGTEXTMANAGER_H
