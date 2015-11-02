#include <stdlib.h>
#include <iostream>
#include <stdio.h>

#include "tests.h"

#include "antivirusManager.h"
#include "folder.h"

bool TESTING = false;

bool antivirusManagerTest()
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
	
	bool res = infected->getVirusState().discovered && !regular->getVirusState().discovered;
	
	delete folder;
	delete am;

	return res;
}

void runTesting()
{
	TESTING = true;
	
	cout << "Datagame testing started..." << endl;
	
	printf("Testing antivirusManager:\n");
	printf("Testing antivirusManager %s\n", antivirusManagerTest() ? "[OK]" : "[FAILED]");
	
	TESTING = false;
	
	exit(0);
}
