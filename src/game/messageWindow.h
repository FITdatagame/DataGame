/**
* @file messageWindow.h
*/

#ifndef MESSAGE_WINDOW_H
#define MESSAGE_WINDOW_H

#include "group.h"

/**
 * @class MessageWindow
 * @brief Slouží k zobrazování oken se zprávami pro hráče.
 */
class MessageWindow : public Group {
public:
	MessageWindow();
	~MessageWindow();


	/** @brief Nastavení zprávy, kterou bude okno zobrazovat.
	*/
	void setMessage(const string & message);

	/** @brief Přidání další možnosti odpovědi. Kód bude vrácen po kliknutí na tuto možnost.
	*/
	void addOption(const string & text, int code);

	/** @brief Vymaže zprávu a možnosti.
	*/
	void clear();


	/** @brief Zobrazí okno.
	*/
	void show();

	/** @brief Skryje okno.
	*/
	void hide();


	/** @brief Nastaví callback, který se zavolá po kliknutí na jednu z možností.
	*/
	void setOnClickFunc(function<void (int code)> fn) { onClickFunc = fn; }

private:

	/** @brief Funkce, která se zavolá po kliknutí na jednu z možných odpovědí.
	*/
	function<void (int code)> onClickFunc;

	/** @brief Pozice pro následující přidanou možnost odpovědi.
	*/
	double optPos;

	/** @brief Vizuální prvky.
	*/
	Group * elements;
};

#endif
