/**
* @file disk.h
*/

#ifndef DISK_H
#define DISK_H

#include <functional>
using namespace std;

#include "folder.h"

/**
* @struct DiskChange
* @brief Struktura uchovává informace o uskutečněné změně na disku.
*/
struct DiskChange {
	enum Type { FOLDER_ADDED, FOLDER_REMOVED, FOLDER_MODIFIED, FILE_ADDED, FILE_REMOVED, FILE_MODIFIED };

	DiskChange(Type type, Folder * folder, File * file) : type(type), folder(folder), file(file)
	{
		if(type == FILE_REMOVED)
		{
			removedFileName = file->getName();
			removedFileData = file->getData();
		}
	}

	Type type;
	Folder * folder = NULL;
	File * file = NULL;
	string removedFileName;
	int removedFileData;
	bool fullDisk = false;
};

/**
* @class Disk
* @brief Představuje disk, na kterém jsou soubory a složky. Buď uživatelská data nebo zálohy.
*/
class Disk
{
public:
	Disk(long long int capacity, const string & folderName, bool readOnly = false);
	~Disk();


	/** @brief Vrátí kapacitu disku.
	*/
	long long int getCapacity() const { return capacity; }

	/** @brief Vrátí, zda je disk jen pro čtení (např. zálohový disk).
	*/
	bool isReadOnly() const { return readOnly; }

	/** @brief Provede zálohu disku na uvedený disk.
	*/
	bool backup(Disk * to);

	/** @brief Výměna starého disku za nový.
	*/
	void onChange(DiskChange & c);

	/** @brief Vrátí kořenový adresář.
	*/
	Folder * getRootFolder() { return rootFolder; }

	/** @brief Nastaví funkci, která se bude volat při změně na disku.
	*/
	void setOnChange(function<void (DiskChange c)> fn) { onChangeFunc = fn; }

private:

	/** @brief Projde rekurzivně všechny soubory ve složce a deaktivuje v nich viry.
	*/
	void deactivateViruses(Folder * folder);

	/** @brief Kapacita disku v bajtech.
	*/
	long long int capacity;

	/** @brief Zdraví disku v rozmezí 0 až 1.
	*/
	double health;

	/** @brief Značí, zda je disk jen pro čtení (zálohový dísk).
	*/
	bool readOnly;

	/** @brief Ukazatel na kořenový adresář na disku.
	*/
	Folder * rootFolder;

	/** @brief Funkce, která se zavolá, pokud nastane na disku změna (souboru nebo složky). V parametru se předá typ změny.
	*/
	function<void (DiskChange c)> onChangeFunc;
};

#endif // DISK_H
