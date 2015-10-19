/**
* @file permissionSelector.h
*/

#ifndef PERMISSIONSELECTOR_H
#define PERMISSIONSELECTOR_H

#include <functional>

#include "group.h"
#include "image.h"
#include "text.h"
#include "file.h"

/**
 * @class PermissionSelector
 * @brief Vytváří okno, ve kterém může hráč měnit oprávnění otevřeného souboru.
 */
class PermissionSelector : public Group {
public:
	PermissionSelector();
	~PermissionSelector();


	/** @brief Otevře soubor pro úpravu práv.
	*/
	void openFile(File * file);


	/** @brief Nastaví funkci, která se zavolá po uložení práv.
	*/
	void setOnClickFunc(function<void (File * file)> func) { onClickFunc = func; }

private:

	/** @brief Mění hodnotu práv na určité pozici.
	*/
	void changeValue(int pos, bool increase);


	/** @brief Obrázek s tabulkou.
	*/
	Image * table;

	/** @brief Pole s tlačítky na ovládání výběru práv.
	*/
	Image * buttons[6];

	/** @brief Obrázek s potvrzovacím tlačítkem.
	*/
	Image * ok;

	/** @brief Texty s číselnou hodnotou složek práv.
	*/
	Text * values[3];


	/** @brief Jednotlivé složky práv.
	*/
	char vals[3];


	/** @brief Otevřený soubor.
	*/
	File * file = NULL;


	/** @brief Funkce, která se zavolá po uložení práv.
	*/
	function<void (File * file)> onClickFunc;
};

#endif // PERMISSIONSELECTOR_H
