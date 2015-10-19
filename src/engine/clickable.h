/**
* @file clickable.h
*/
/* Clickable objects interface. These are the necessary functions to be implemented
 * so that the Input handler can inform objects of being clicked or pointed at.
 */

#ifndef CLICKABLE_H
#define CLICKABLE_H

#include <functional>
#include "engine.h"
#include "object.h"

/**
 * @class Clickable
 * @brief Rozhraní umožňující nastavovat funkce, které se mají vykonat při událostech kliknutí nebo najetí myší na objekt, který toto rozhraní podědí.
 */
class Clickable : public Object {
public:
	Clickable() : onReleaseFunc(NULL), onClickFunc(NULL), onPointFunc(NULL), onInFunc(NULL), onOutFunc(NULL), ignorePos(false) {}

	/**
	 * @brief Nastaví funkci, která se vykoná po kliknutí.
	 * @param func
	 */
	void setOnClickFunc(function<bool (int x, int y, bool right)> func) { onClickFunc = func; }

	/**
	 * @brief Nastaví funkci, která se vykoná po puštění tlačítka myši.
	 * @param func
	 */
	void setOnReleaseFunc(function<bool (int x, int y)> func) { onReleaseFunc = func; }

	/**
	 * @brief Nastaví funkci, která se vykoná, když se myš pohybuje nad objektem.
	 * @param func
	 */
	void setOnPointFunc(function<bool (int x, int y)> func) { onPointFunc = func; }

	/**
	 * @brief Nastaví funkci, která se vykoná při najetí myši.
	 * @param func
	 */
	void setOnInFunc(function<bool (int x, int y)> func) { onInFunc = func; }

	/**
	 * @brief Nastaví funkci, která se vykoná po sjetí myši.
	 * @param func
	 */
	void setOnOutFunc(function<bool (int x, int y)> func) { onOutFunc = func; }

	/**
	 * @brief Nastaví funkci, která se vykoná při zadávání textu.
	 * @param func
	 */
	void setOnTextFunc(function<bool (const char * txt)> func) { onTextFunc = func; }

	/**
	 * @brief Nastaví funkci, která se vykoná při stisknutí klávesy.
	 * @param func
	 */
	void setKeyDownFunc(function<bool (const SDL_Scancode &)> func) { onKeyDownFunc = func; }

	/**
	 * @brief Nastaví funkci, která se vykoná při scrollování.
	 * @param func
	 */
	void setOnScrollFunc(function<bool (int sc)> func) { onScrollFunc = func; }

	bool onRelease(int x, int y) { if(onReleaseFunc) return onReleaseFunc(x, y); return false; }
	bool onClick(int x, int y, bool right) { if(onClickFunc) return onClickFunc(x, y, right); return false; }
	bool onPoint(int x, int y) { if(onPointFunc) return onPointFunc(x, y); return false; }
	bool onIn(int x, int y) { if(onInFunc) return onInFunc(x, y); return false; }
	bool onOut(int x, int y) { if(onOutFunc) return onOutFunc(x, y); return false; }

	bool onText(const char * txt) { if(onTextFunc) return onTextFunc(txt); return false; }
	bool onKeyDown(const SDL_Scancode & scancode) { if(onKeyDownFunc) return onKeyDownFunc(scancode); return false; }
	bool onScroll(int sc) { if(onScrollFunc) return onScrollFunc(sc); return false; }

	/**
	 * @brief Funkce dostane souřadnice x, y a vrací true, pokud se na těchto souřadnicích vyskytuje daný objekt. Funkce je abstraktní a musí být implementovaná v potomcích.
	 * @param x
	 * @param y
	 * @return
	 */
	virtual bool isAt(double x, double y) = 0;

	/**
	 * @brief Volá se rekurzivně na celý strom grafu, pokud nastane jakýkoliv vstup.
	 * @param fn
	 * @return
	 */
	virtual bool input(function<bool (Clickable * o)> fn) { return enabled ? fn(this) : false; }

	void ignoreEventPosition(bool ign) { ignorePos = ign; }
	bool getIgnoreEventPosition() { return ignorePos; }

protected:
	function<bool (int x, int y)> onReleaseFunc;
	function<bool (int x, int y, bool right)> onClickFunc;
	function<bool (int x, int y)> onPointFunc;
	function<bool (int x, int y)> onInFunc;
	function<bool (int x, int y)> onOutFunc;

	function<bool (const char * txt)> onTextFunc;
	function<bool (const SDL_Scancode &)> onKeyDownFunc;
	function<bool (int sc)> onScrollFunc;

	bool ignorePos;
};

#endif // CLICKABLE_H
