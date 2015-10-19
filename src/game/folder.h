/**
* @file folder.h
*/

#ifndef FOLDER_H
#define FOLDER_H

#include <string>
#include <set>
#include <vector>
#include <algorithm>
using namespace std;

#include "file.h"

class Disk;

/**
* @struct FolderComparator
* @brief Slouží pro řazení složek podle jména.
*/
struct FolderComparator {
	bool operator() (Folder * const & lhs, Folder * const & rhs) const;
};

/**
* @class Folder
* @brief Představuje jednu složku v systému souborů.
*/
class Folder
{
public:
	static const int CAN_BE_COMPRESSED = 1;
	static const int CAN_BE_RENAMED = 2;
	static const int CAN_BE_REMOVED = 4;
	static const int ALL_FLAGS = 7;

	Folder(const string & name) : name(name), compressed(false), collapsed(false), flags(ALL_FLAGS), parent(NULL), disk(NULL) {}
	Folder(const Folder & f);
	~Folder();

	/** @brief Přejmenuje složku (včetně přeřazení podle abecedy).
	*/
	void setName(const string & n);

	/** @brief Vrátí jméno složky.
	*/
	string getName() const { return name; }


	/** @brief Vrátí, zda je složka v zobrazení otevřená.
	*/
	bool isCollapsed() const { return collapsed; }

	/** @brief Nastaví otevřenost složky v zobrazení ve správci složek.
	*/
	void setCollapsed(bool c) { collapsed = c; }


	/** @brief Vrátí, zda je složka komprimovaná.
	*/
	bool isCompressed() const { return compressed; }

	/** @brief Komprimuje/dekomprimuje složku.
	*/
	void setCompressed(bool c);


	/** @brief Vrátí hodnotu požadovaných vlajek.
	*/
	int getFlag(int f) const { return flags & f; }

	/** @brief Nastaví hodnotu požadovaných vlajek na true.
	*/
	void setFlag(int f) { flags |= f; }

	/** @brief Nastaví hodnotu požadovaných vlajek na false.
	*/
	void unsetFlag(int f) { flags &= ~f; }


	/** @brief Rekurzivní funkce, která do výstupního parametru rozbalí (linearizuje) adresářovou strukturu všech podsložek. Zároveň vrací i index (hloubku) složek.
	*/
	void getAllFoldersRecursively(vector<pair<Folder *, int>> & folds, bool withCollapsed = true, int idx = 0);

	/** @brief Rekurzivní funkce, která vrátí seznam všech souborů, které se nachází v ní a v podsložkách.
	*/
	void getAllFilesRecursively(vector<File*> & fls);


	/** @brief Přidá soubor do složky. Vrací bool podle úspěchu.
	*/
	bool addFile(File * file);

	/** @brief Vratí ukazatel na soubor podle jména. Pokud není nalezen, vrací NULL.
	*/
	File * getFile(const string & file);

	/** @brief Vrací referenci na množinu souborů ve složce.
	*/
	const set<File*, FileComparator> & getFiles() const { return files; }

	/** @brief Odebere soubor podle jména ze složky. Vrací bool podle úspěchu.
	*/
	bool removeFile(const string & file);


	/** @brief Přidá složku do této složky. Vrací bool podle úspěchu.
	*/
	bool addFolder(Folder * folder);

	/** @brief Vrátí ukazatel na složku podle jména. Pokud není nalezena, vrátí NULL.
	*/
	Folder * getFolder(const string & folder);

	/** @brief Vrací referenci na množinu složek.
	*/
	const set<Folder*, FolderComparator> & getFolders() const { return folders; }

	/** @brief Odstraní složku podle jména. Vrací bool podle úspěchu.
	*/
	bool removeFolder(const string & folder);


	/** @brief Vrátí velikost dané složky.
	*/
	long long int getSize() const;


	/** @brief Vrátí rodičovský adresář.
	*/
	Folder * getParent() { return parent; }

	/** @brief Vrátí disk, na kterém se nachází.
	*/
	Disk * getDisk() { return disk; }

	/** @brief Nastaví disk složky.
	*/
	void setDisk(Disk * d);

private:

	/** @brief Rekurzivní funkce nastavující kompresi.
	*/
	void setCompressedInternal(bool c);


	/** @brief Jméno složky.
	*/
	string name;

	/** @brief Je zkomprimovaná?
	*/
	bool compressed;

	/** @brief Je zavřená v průzkumníku složek?
	*/
	bool collapsed;

	/** @brief Vlajky určují možné operace se složkou.
	*/
	int flags;


	/** @brief Množina souborů ve složce.
	*/
	set<File*, FileComparator> files;

	/** @brief Množina složek ve složce.
	*/
	set<Folder*, FolderComparator> folders;


	/** @brief Ukazatel na rodičovskou složku.
	*/
	Folder * parent;

	/** @brief Ukazatel na disk, na kterém je složka uložena.
	*/
	Disk * disk;
};

#endif // FOLDER_H
