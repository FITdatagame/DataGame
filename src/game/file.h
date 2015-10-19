/**
* @file file.h
*/


#ifndef FILE_H
#define FILE_H

#include <string>
using namespace std;

#include "virus.h"

class Folder;

/**
* @class File
* @brief Představuje soubor v systému souborů.
*/
class File
{
public:
	enum Type { EXECUTABLE, TEXT, IMAGE, VIDEO };

	File();
	File(const File & f);
	~File();

	/** @brief Nastaví nové jméno.
	*/
	void setName(const string & name);

	/** @brief Vrátí jméno souboru.
	*/
	string getName() const { return name; }

	/** @brief Vrátí typ souboru.
	*/
	Type getType() const { return type; }

	/** @brief Nastaví práva souboru.
	*/
	void setPermissions(int p);

	/** @brief Vrátí práva souboru.
	*/
	int getPermissions() const { return permissions; }

	/** @brief Zkomprimuje nebo dekomprimuje soubor.
	*/
	void setCompressed(bool c) { compressed = c; }

	/** @brief Vrátí, zda je soubor zkomprimovaný.
	*/
	bool isCompressed() const { return compressed; }

	/** @brief Vrátí data obsažená v souboru.
	*/
	int getData() const { return data; }

	/** @brief Vloží virus do souboru.
	*/
	void setVirus(Virus * v);

	/** @brief Vrátí status viru v souboru (i pokud v něm virus není).
	*/
	VirusState & getVirusState() { return virusState; }

	/** @brief Vrátí aktuální velikost souboru s ohledem na komprimaci.
	*/
	long long int getSize() const { return size / (compressed ? compressionRatio : 1); }

	/** @brief Vrátí skutečnou velikost souboru bez ohledu na komprimaci.
	*/
	long long int getRealSize() const { return size; }

	/** @brief Nastaví rodičovský adresář.
	*/
	void setFolder(Folder * f) { folder = f; }

	/** @brief Vrátí rodičovský adresář.
	*/
	Folder * getFolder() { return folder; }

protected:

	/** @brief Náhodně vygeneruje jméno souboru.
	*/
	void genName();

	/** @brief Jméno souboru.
	*/
	string name;

	/** @brief Typ souboru: EXECUTABLE, TEXT, IMAGE nebo VIDEO.
	*/
	Type type;

	/** @brief Práva souboru.
	*/
	int permissions;

	/** @brief Velikost souboru v bajtech.
	*/
	long long int size;

	/** @brief Maximální kompresní poměr pro tento soubor. Číslo 1.0 až N.
	*/
	double compressionRatio;

	/** @brief Je soubor zkomprimován?
	*/
	bool compressed;

	/** @brief Ukazatel na rodičovskou složku.
	*/
	Folder * folder;

	/** @brief Stav viru, který je (nebo není) v souboru.
	*/
	VirusState virusState;

	/** @brief Data obsažená v souboru.
	*/
	int data;
};

/**
* @struct FileComparator
* @brief Slouží k řazení souborů podle jejich jména.
*/
struct FileComparator {
	bool operator() (File * const & lhs, File * const & rhs) const { return lhs->getName() < rhs->getName(); }
};

#endif // FILE_H
