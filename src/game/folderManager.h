/**
* @file folderManager.h
*/

#ifndef FOLDERMANAGER_H
#define FOLDERMANAGER_H

#include <vector>
#include <functional>
using namespace std;

#include "folder.h"
#include "group.h"
#include "image.h"
#include "text.h"
#include "inputDialog.h"
#include "rectangle.h"

/**
* @class FolderManager
* @brief Zobrazuje strom složek na uživatelském disku. Hráč může složky rozbalovat a sbalovat.
*/
class FolderManager : public Group {
public:
	FolderManager(int width, int height);
	~FolderManager();


	/** @brief Funkce zobrazí adresářovou strukturu začínající složkou rootFolder.
	*/
	void display(Folder * rootFolder);


	/** @brief Vrátí ukazatel na otevřenou složku.
	*/
	Folder * getFolder() { return rootFolder; }

	/** @brief Vrátí ukazatel na vybranou složku.
	*/
	Folder * getSelected() { return selected; }


	/** @brief Nastaví funkci, která se bude volat po kliknutí na složku..
	*/
	void setOnClickFunc(function<void (Folder * folder)> func) { onClickFunc = func; }

private:

	/** @brief Funkce volaná pravidelně v každém framu.
	*/
	virtual void update();


	/** @brief Překreslí okno.
	*/
	void doDisplay();

	/** @brief Zobrazuje kontextové menu.
	*/
	void showContextMenu();

	/** @brief Skrývá kontextové menu.
	*/
	void hideContextMenu();


	/** @brief Aktuálně vybraná složka.
	*/
	Folder * selected = NULL;

	/** @brief Určuje, zda se v příštím updatu překreslí okno.
	*/
	bool prepareDisplay = false;


	/** @brief Funkce, která se zavolá po kliknutí na složku.
	*/
	function<void (Folder * folder)> onClickFunc;


	/** @brief Aktuálně zobrazovaná složka.
	*/
	Folder * rootFolder;


	/** @brief Grafický prvek představující kontextové menu.
	*/
	Group * contextMenu;

	/** @brief Grafický prvek představující strom složek.
	*/
	Group * folderGroup;

	/** @brief Grafický prvek představující pozadí.
	*/
	Rectangle * background;

	/** @brief Aktuální pozice v seznamu složek (pro scrollování).
	*/
	int topFolderPos;

	/** @brief Text zobrazující zaplnění disku.
	*/
	Text * diskState;
};

#endif // FOLDERMANAGER_H
