/**
* @file game.h
*/

#ifndef GAME_H
#define GAME_H

#include "group.h"
#include "folderManager.h"
#include "fileManager.h"
#include "permissionSelector.h"
#include "disk.h"
#include "movingTextManager.h"
#include "messageWindow.h"
#include "taskManager.h"
#include "firewallManager.h"
#include "antivirusManager.h"

/**
* @class Game
* @brief Hlavní třída, která řídí chod celé hry.
*/
class Game : public Group {
public:
	Game();
	~Game();


	/** @brief Spustí hru.
	*/
	void run() { pauseText->setVisible(false); isRunning = true; }

	/** @brief Pozastaví hru.
	*/
	void pause() { pauseText->setVisible(true); isRunning = false; }


	/** @brief Volá se v každém cyklu a řídí průběh hry.
	*/
	virtual void update();

private:

	/** @brief Hlavní disk pro uživatele.
	*/
	Disk * userDisk;

	/** @brief Disk určený na zálohy.
	*/
	Disk * backupDisk;

	/** @brief Zobrazuje hierarchii složek.
	*/
	FolderManager * folderManager;

	/** @brief Zobrazuje soubory ve složce.
	*/
	FileManager * fileManager;

	/** @brief Upravuje práva souborů.
	*/
	PermissionSelector * selector;

	/** @brief Objekt ovládající běžící text ve spod obrazovky.
	*/
	MovingTextManager * movingText;

	/** @brief Tlačítko na zobrazení zálohovacího disku.
	*/
	Text * backupDiskButton;

	/** @brief Tlačítko na zobrazení uživatelského disku.
	*/
	Text * userDiskButton;

	/** @brief Tlačítko na zálohování disku.
	*/
	Text * backupButton;

	/** @brief Tlačítko na zobrazení úkolů.
	*/
	Text * tasksButton;

	/** @brief Tlačítko na zobrazení správce firewallu.
	*/
	Text * firewallButton;

	/** @brief Tlačítko na spuštění antiviru.
	*/
	Text * antivirusButton;

	/** @brief Text oznamující, že hra byla zapauzovaná.
	*/
	Text * pauseText;

	/** @brief Uvítací okno se zprávou.
	*/
	MessageWindow * welcomeMessage;

	/** @brief Správce úkolů a skóre.
	*/
	TaskManager * taskManager;

	/** @brief Správce firewallu.
	*/
	FirewallManager * firewallManager;

	/** @brief Správce antiviru.
	*/
	AntivirusManager * antivirusManager;


	/** @brief Určuje, zda jsme na zálohovacím nebo uživatelském disku.
	*/
	bool backupShown = false;

	/** @brief Určuje, zda hra běží, nebo je pozastavena.
	*/
	bool isRunning = false;

	/** @brief Určuje, jestli se v příštím updatu překreslí okna.
	*/
	bool redrawRequired = false;

	/** @brief Počítá dobu, po kterou působí antivirus.
	*/
	double antivirusFreezeTimeout = 0;
};

#endif // GAME_H
