#include "disk.h"

#include <ctime>

#include "virus.h"

/**
 * @brief Při vytvoření nového disku rovnou přidáme kořenový adresář.
 * @param capacity kapacita disku
 * @param folderName název kořenového adresáře
 * @param readOnly je disk pouze pro čtení?
 */
Disk::Disk(long long capacity, const string & folderName, bool readOnly) : capacity(capacity), readOnly(readOnly)
{
	rootFolder = new Folder(folderName);
	rootFolder->setDisk(this);
	rootFolder->unsetFlag(Folder::CAN_BE_COMPRESSED | Folder::CAN_BE_REMOVED | Folder::CAN_BE_RENAMED);
}

Disk::~Disk()
{
	delete rootFolder;
}

void Disk::deactivateViruses(Folder * folder)
{
	for(auto & f : folder->getFiles())
		if(f->getVirusState().active)
			f->getVirusState().virus->fileDeactivated();

	for(auto & f : folder->getFolders())
		deactivateViruses(f);
}

bool Disk::backup(Disk * to)
{
	time_t rawtime;
	struct tm * timeinfo;
	char buf[64];

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buf, 64, "zaloha_%d-%m-%y_%H-%I-%S", timeinfo);

	Folder * backupFolder = new Folder(buf);
	Folder * rootFolderBackup = new Folder(*rootFolder);

	backupFolder->addFolder(rootFolderBackup);
	backupFolder->setCompressed(true);
	backupFolder->setCollapsed(true);

	int backupSize = backupFolder->getSize();
	if(to->capacity < backupSize)
	{
		delete backupFolder;
		return false;
	}

	bool old = to->readOnly;
	to->readOnly = false;

	while(backupSize > to->capacity - to->getRootFolder()->getSize())
	{
		Folder * first = *to->getRootFolder()->getFolders().begin();
		to->getRootFolder()->removeFolder(first->getName());
		delete first;
	}

	to->getRootFolder()->addFolder(backupFolder);
	deactivateViruses(backupFolder);

	to->readOnly = old;

	DiskChange tmp(DiskChange::FOLDER_MODIFIED, backupFolder, NULL);
	onChange(tmp);

	return true;
}

void Disk::onChange(DiskChange & c)
{
	if(DiskChange::FILE_ADDED == c.type) //pokud je pridan soubor
	{
		Folder * parent = c.folder;

		if(capacity <= rootFolder->getSize()) //pokud je kapacita mensi nez velikost root folderu
		{
			parent->removeFile(c.file->getName());
			c.fullDisk = true;
		}
	}

	if(onChangeFunc) onChangeFunc(c);
}
