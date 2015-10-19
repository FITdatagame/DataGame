/**
* @file fileManager.h
*/

#ifndef FILEVIEWER_H
#define FILEVIEWER_H

#include <vector>
#include <functional>
using namespace std;

#include "group.h"
#include "folder.h"
#include "utils.h"
#include "rectangle.h"
#include "text.h"

/**
* @class FileManager
* @brief Průzkumník souborů, umožňuje kopírování, vkládání a mazání.
*/
class FileManager : public Group {
public:
	FileManager(int width, int height);
	~FileManager();

	/** @brief Zobrazí obsah (pouze soubory) zadané složky.
	*/
	void display(Folder * folder);

	/** @brief Vyčistí okno se soubory.
	*/
	void clearView();


	/** @brief Vrátí ukazatel na otevřenou složku.
	*/
	Folder * getFolder() { return folder; }

	/** @brief Vrátí ukazatel na vybraný soubor.
	*/
	File * getSelected() { return selected; }


	/** @brief Nastaví funkci, která se vykoná po kliknutí na soubor.
	*/
	void setOnClickFunc(function<void (File * file)> func) { onClickFunc = func; }

private:

	/** @brief Zobrazí kontextové menu.
	*/
	void showContextMenu();

	/** @brief Skryje kontextové menu.
	*/
	void hideContextMenu();

	/** @brief Vypíše práva v textové podobě.
	*/
	string parsePermissions(int permissions);


	/** @brief Adresářová cesta k souboru, který byl zkopírován do schránky.
	*/
	FilePath clipboard;

	/** @brief Značí, zda soubor ve schránce jen kopírujeme nebo vyjímáme.
	*/
	bool copy = false;


	/** @brief Otevřená složka.
	*/
	Folder * folder = NULL;

	/** @brief Aktuálně vybraný soubor.
	*/
	File * selected = NULL;

	/** @brief Grafický prvek představující seznam souborů.
	*/
	Group * fileGroup;

	/** @brief Obsahuje objekt s kontextovým menu.
	*/
	Group * contextMenu;

	/** @brief Grafický prvek představující pozadí.
	*/
	Rectangle * background;

	/** @brief Aktuální pozice v seznamu souborů (pro scrollování).
	*/
	int topFilePos;

	/** @brief Funkce, která se vykoná po kliknutí na soubor.
	*/
	function<void (File * file)> onClickFunc;
};

#endif // FILEVIEWER_H
