/**
* @file virus.h
*/

#ifndef VIRUS_H
#define VIRUS_H

#include <iostream>
using namespace std;

class Folder;
class File;

/**
 * @class Virus
 * @brief Reprezentace viru.
 */
class Virus
{
public:
	enum Type { ERASING, CREATING, MODIFYING };

	Virus();
	~Virus() { cout << "virus deleted" << endl; }


	/** @brief Volá se v každém cyklu hry. Pokud byl vir nečinný, vrací false. Pokud škodil, vrací true.
	* Parametrem je kořenová složka, ve které má vir škodit.
	*/
	bool update(Folder * rootFolder);


	/** @brief Voláním oznamujeme viru, že se rozšířil do dalšího souboru.
	*/
	void fileAdded() { if(++filesCnt > 0 && activeFilesCnt > 0) active = true; cout << "virus file added, active " << active << endl; }

	/** @brief Voláním oznamujeme, že byl smazán jeden soubor, ve kterém vir žil.
	*/
	void fileRemoved() { if(--filesCnt <= 0 || activeFilesCnt <= 0) active = false; cout << "virus file removed, active " << active << endl;  }


	/** @brief Voláním oznamujeme, že byl aktivován (obnoven ze zálohy či vytvořen) další soubor, ve kterém je virus zakořeněný.
	*/
	void fileActivated() { if(++activeFilesCnt > 0) active = true; cout << "virus file activated, active " << active << endl;  }

	/** @brief Voláním oznamujeme, že jeden ze souborů, ve kterém je virus zakořeněný, byl deaktivován (smazán).
	*/
	void fileDeactivated() { if(--activeFilesCnt <= 0) active = false; cout << "virus file deactivated, active " << active << endl;  }


	/** @brief Pokud je vir zakořeněný aspoň v jednom souboru, vrací true.
	*/
	bool isAlive() { return filesCnt > 0; }

private:

	/** @brief Udává, zda je vir aktivní (může škodit).
	*/
	bool active;

	/** @brief Jeden ze tří typů viru: mazací, vytvářecí a upravující soubory.
	*/
	Type type;

	/** @brief Počet sekund do příštího škodění.
	*/
	double nextHarm;


	/** @brief Počet souborů, ve kterých je virus zakořeněný.
	*/
	int filesCnt = 0;

	/** @brief Počet aktivních (ne v záloze) souborů, ve kterých je virus zakořeněný.
	*/
	int activeFilesCnt = 0;
};

/**
* @struct VirusState
* @brief Struktura, která obsahuje stav viru v souboru.
*/
struct VirusState {
	Virus * virus = NULL; /*!< Ukazatel na virus. */
	bool active = false; /*!< Určuje, zda je virus v souboru aktivní. (neaktivní je pokud je soubor v záloze) */
	bool discovered = false; /*!< Určuje, zda byl virus odhalen. */
};

#endif // VIRUS_H
