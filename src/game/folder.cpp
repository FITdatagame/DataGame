#include "folder.h"

#include "disk.h"

bool FolderComparator::operator() (Folder * const & lhs, Folder * const & rhs) const
{
	string n1 = lhs->getName();
	string n2 = rhs->getName();

	transform(n1.begin(), n1.end(), n1.begin(), ::tolower);
	transform(n2.begin(), n2.end(), n2.begin(), ::tolower);

	return n1 < n2;
}

Folder::Folder(const Folder & f)
{
	name = f.name;
	compressed = f.compressed;
	collapsed = f.collapsed;
	disk = f.disk;
	flags = ALL_FLAGS;

	for(auto & file : f.files)
	{
		File * newFile = new File(*file);
		newFile->setCompressed(compressed);
		newFile->setFolder(this);
		files.insert(newFile);
	}

	for(auto & folder : f.folders)
	{
		Folder * newFolder = new Folder(*folder);
		if(compressed) newFolder->setCompressed(compressed);
		newFolder->parent = this;
		folders.insert(newFolder);
	}
}

Folder::~Folder()
{
	for(auto f : folders) delete f;
	for(auto f : files) delete f;
}

void Folder::setName(const string & n)
{
	if(!getFlag(CAN_BE_RENAMED)) return;

	if(parent)
	{
		parent->folders.erase(this);
		name = n;
		parent->folders.insert(this);
	}
	else name = n;
	DiskChange tmp(DiskChange::FOLDER_MODIFIED, this, NULL);
	if(disk) disk->onChange(tmp);
}

bool Folder::addFile(File * file)
{
	if(disk && disk->isReadOnly()) return false;

	auto res = files.insert(file);

	if(res.second)
	{
		file->setFolder(this);
		file->setCompressed(compressed);
		DiskChange tmp(DiskChange::FILE_ADDED, this, file);
		if(disk) disk->onChange(tmp);
	}

	return res.second;
}

File * Folder::getFile(const string & file)
{
	File tmpFile;
	tmpFile.setName(file);

	auto it = files.find(&tmpFile);
	return it != files.end() ? *it : NULL;
}

bool Folder::removeFile(const string & file)
{
	if(disk && disk->isReadOnly()) return false;

	File * f = getFile(file);

	if(f)
	{
		files.erase(f);
		f->setFolder(NULL);
		f->setCompressed(false);
		DiskChange tmp(DiskChange::FILE_REMOVED, this, f);
		if(disk) disk->onChange(tmp);
	}

	return f;
}

bool Folder::addFolder(Folder * folder)
{
	if(disk && disk->isReadOnly()) return false;

	auto res = folders.insert(folder);

	if(res.second)
	{
		folder->parent = this;
		folder->setDisk(disk);
		if(compressed) folder->setCompressed(compressed);
		DiskChange tmp(DiskChange::FOLDER_ADDED, folder, NULL);
		if(disk) disk->onChange(tmp);
		return true;
	}
	else return false;
}

Folder * Folder::getFolder(const string & folder)
{
	Folder tmpFolder(folder);

	auto it = folders.find(&tmpFolder);
	return it != folders.end() ? *it : NULL;
}

bool Folder::removeFolder(const string & folder)
{
	if(disk && disk->isReadOnly()) return false;

	Folder tmpFolder(folder);
	auto it = folders.find(&tmpFolder);

	if(it != folders.end())
	{
		Folder * f = *it;
		folders.erase(it);
		DiskChange tmp(DiskChange::FOLDER_REMOVED, f, NULL);
		if(disk) disk->onChange(tmp);
		return true;
	}

	return false;
}

void Folder::setCompressed(bool c)
{
	if(!getFlag(CAN_BE_COMPRESSED)) return;
	if(disk && disk->isReadOnly()) return;

	setCompressedInternal(c);
	DiskChange tmp(DiskChange::FOLDER_MODIFIED, this, NULL);
	if(disk) disk->onChange(tmp);
}

void Folder::setCompressedInternal(bool c)
{
	compressed = c;

	for(auto f : folders) f->setCompressed(c);
	for(auto f : files) f->setCompressed(c);
}

void Folder::getAllFoldersRecursively(vector<pair<Folder *, int>> & folds, bool withCollapsed, int idx)
{
	folds.push_back(pair<Folder*, int>(this, idx));

	if(withCollapsed || !collapsed)
		for(auto f : folders)
			f->getAllFoldersRecursively(folds, withCollapsed, idx + 1);
}

void Folder::getAllFilesRecursively(vector<File *> & fls)
{
	for(auto & f : files) fls.push_back(f);
	for(auto & f : folders) f->getAllFilesRecursively(fls);
}

long long int Folder::getSize() const
{
	long long int sz = 0;

	for(auto f : files) sz += f->getSize();
	for(auto f : folders) sz += f->getSize();

	return sz;
}

void Folder::setDisk(Disk * d)
{
	disk = d;

	for(auto f : folders) f->setDisk(d);
}
