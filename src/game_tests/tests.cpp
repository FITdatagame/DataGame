#include <stdlib.h>
#include <iostream>
#include <stdio.h>

#include "tests.h"

#include "antivirusManager.h"
#include "folder.h"
#include "disk.h"

bool TESTING = false;

string antivirusManagerTest()
{
	AntivirusManager * am = new AntivirusManager();
	
	Folder * folder = new Folder("test");
	File * regular = new File();
	File * infected = new File();
	Virus * virus = new Virus();
	
	infected->setVirus(virus);
	
	folder->addFile(regular);
	folder->addFile(infected);
	
	am->processFolder(folder);
	
	if(!infected->getVirusState().discovered || regular->getVirusState().discovered)
		return "virus detection failed";
	
	delete folder;
	delete am;

	return "";
}

string diskTest()
{
	Disk * disk = new Disk(1000000000, "test", false);
	
	if(disk->isReadOnly()) return "readonly check failed";
	if(disk->getCapacity() != 1000000000) return "capacity check failed";
	if(!disk->getRootFolder()) return "root folder was not generated";
	if(disk->getRootFolder()->getName() != "test") return "root folder name check failed";
	
	File * infected = new File();
	Virus * virus = new Virus();
	
	infected->setVirus(virus);
	disk->getRootFolder()->addFile(infected);
	
	Disk * backup = new Disk(1000000000, "b", true);
	if(!disk->backup(backup)) return "backup failed";
	
	vector<pair<Folder *, int>> folds;
	backup->getRootFolder()->getAllFoldersRecursively(folds);
	if(!folds.size()) return "backup was not created";
	
	delete disk;
	delete backup;
	
	return "";
}

string fileTest()
{
	File * f = new File();
	
	if(f->getName() == "") return "filename gen failed";
	
	f->setCompressed(true);
	
	if(f->getSize() >= f->getRealSize()) return "compression failed";
	
	Virus * virus = new Virus();
	f->setVirus(virus);
	
	delete f;
	
	if(virus->isAlive()) return "virus survived after file deletion";

	return "";
}

string folderTest()
{
	vector<pair<Folder *, int>> folds;
	vector<File*> fls;
	Folder * f = new Folder("test");
	
	f->getAllFoldersRecursively(folds);
	if(folds.size() > 1) return "found folders in empty folder";
	
	f->getAllFilesRecursively(fls);
	if(fls.size()) return "files found in empty folder";
	
	File * file = new File();
	f->addFile(file);
	
	fls.clear();
	f->getAllFilesRecursively(fls);
	if(fls.size() != 1) return "file count mismatch";
	if(f->getSize() != file->getSize()) return "file size mismatch";
	
	Folder * f2 = new Folder("test2");
	if(!f->addFolder(f2)) return "addfolder failed";
	
	folds.clear();
	f->getAllFoldersRecursively(folds);
	if(folds.size() != 2) return "folder count mismatch";
	
	File * file2 = new File();
	f2->addFile(file2);
	
	fls.clear();
	f->getAllFilesRecursively(fls);
	if(fls.size() != 2) return "file count mismatch2";
	if(f->getSize() != file->getSize() + file2->getSize()) return "file size mismatch2";
	
	f->removeFile(file->getName());
	delete file;
	
	fls.clear();
	f->getAllFilesRecursively(fls);
	if(fls.size() != 1) return "file count mismatch3";
	if(f->getSize() != file2->getSize()) return "file size mismatch3";
	
	f->removeFolder(f2->getName());
	delete f2;
	
	folds.clear();
	f->getAllFoldersRecursively(folds);
	if(folds.size() != 1) return "folder count mismatch2";
	
	delete f;

	return "";
}

void runTesting()
{
	TESTING = true;
	
	bool res = true, test;
	string out;
	cout << "Datagame testing started..." << endl;
	
	printf("Testing antivirusManager:\n");
	out = antivirusManagerTest();
	res &= test = out == string("");
	if(!test) printf("%s\n", out.c_str());
	printf("Testing antivirusManager %s\n", test ? "[OK]" : "[FAILED]");
	
	printf("\nTesting disk:\n");
	out = diskTest();
	res &= test = out == string("");
	if(!test) printf("%s\n", out.c_str());
	printf("Testing disk %s\n", test ? "[OK]" : "[FAILED]");
	
	printf("\nTesting file:\n");
	out = fileTest();
	res &= test = out == string("");
	if(!test) printf("%s\n", out.c_str());
	printf("Testing file %s\n", test ? "[OK]" : "[FAILED]");
	
	printf("\nTesting folder:\n");
	out = folderTest();
	res &= test = out == string("");
	if(!test) printf("%s\n", out.c_str());
	printf("Testing folder %s\n", test ? "[OK]" : "[FAILED]");
	
	TESTING = false;
	
	exit(res ? 0 : 1);
}
