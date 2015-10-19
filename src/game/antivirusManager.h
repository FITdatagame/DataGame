/**
* @file antivirusManager.h
*/

#ifndef ANTIVIRUSMANAGER_H
#define ANTIVIRUSMANAGER_H

#include "folder.h"
#include "group.h"
#include "messageWindow.h"

/**
* @class AntivirusManager
* @brief Umožňuje skenovat soubory a najít v nich viry.
*/
class AntivirusManager : public Group {
public:
	~AntivirusManager();

	/** @brief Oskenuje danou složku a najde v ní viry.
	*/
	void processFolder(Folder * folder);

	/** @brief Nastavení callbacku při dokončení skenu.
	*/
	void setOnScanComplete(function<void(int res)> fn) { onScanComplete = fn; }

	/** @brief Funkce pravidelně volaná v každém framu.
	*/
	virtual void update();

private:
	double scanTimeout;
	int scanResult;
	MessageWindow * msg = NULL;
	function<void(int res)> onScanComplete;
};

#endif
