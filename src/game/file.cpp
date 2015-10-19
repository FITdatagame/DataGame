#include "file.h"

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

#include "engine.h"
#include "virus.h"
#include "disk.h"

/*
 * Soubory se budou generovat náhodně. Proto při vytvoření souboru
 * náhodně zvolíme jeho typ a další parametry.
 */
File::File() : compressed(false), folder(NULL)
{
	type = (Type)(rand() % 4);

	switch(type)
	{
	case EXECUTABLE:
		permissions = 755;
		size = Engine::random(1000000, 10000000); // min 1MB, max 10MB
		compressionRatio = Engine::random(150, 300) / 100.0; // 1.5 - 3.5
		genName();
		name += ".exe";
		break;
	case TEXT:
		permissions = 644;
		size = Engine::random(1000, 100000); // min 1kB, max 100kB
		compressionRatio = Engine::random(100, 200) / 10.0; // 10.0 - 20.0
		genName();
		name += ".txt";
		break;
	case IMAGE:
		permissions = 644;
		size = Engine::random(100000, 10000000); // min 100kB, max 10MB
		compressionRatio = Engine::random(100, 130) / 100.0; // 1.0 - 1.3
		genName();
		name += ".img";
		break;
	case VIDEO:
		permissions = 644;
		size = Engine::random(100000000, 1000000000); // min 100MB, max 1GB
		compressionRatio = Engine::random(100, 110) / 100.0; // 1.0 - 1.1
		genName();
		name += ".avi";
		break;
	}

	// v 10% případů vygenerujeme náhodná práva
	if(Engine::random(0, 1000000) > 900000)
		permissions = Engine::random(0, 7)*100 + Engine::random(0, 7)*10 + Engine::random(0, 7);

	data = Engine::random(0, 2000000000);
}

File::File(const File & f)
{
	name = f.name;
	type = f.type;
	permissions = f.permissions;
	size = f.size;
	compressionRatio = f.compressionRatio;
	compressed = f.compressed;
	folder = NULL;
	data = f.data;

	if(f.virusState.virus)
	{
		setVirus(f.virusState.virus);
		virusState.discovered = f.virusState.discovered;
	}
}

File::~File()
{
	if(virusState.virus)
	{
		virusState.virus->fileRemoved();
		if(virusState.active) virusState.virus->fileDeactivated();
	}
}

void File::setName(const string & n)
{
	if(folder)
	{
		Folder * f = folder;

		f->removeFile(name);
		name = n;
		f->addFile(this);

		DiskChange tmp(DiskChange::FILE_MODIFIED, folder, this);
		folder->getDisk()->onChange(tmp);
	}
	else name = n;
}

void File::setPermissions(int p)
{
	permissions = p;

	
	if(folder)
	{
		DiskChange tmp(DiskChange::FILE_MODIFIED, folder, this);
		folder->getDisk()->onChange(tmp);
	}
}

void File::setVirus(Virus * v)
{
	if(virusState.virus)
	{
		virusState.virus->fileRemoved();
		if(virusState.active) virusState.virus->fileDeactivated();
	}

	virusState.virus = v;
	virusState.active = true;
	virusState.discovered = false;

	v->fileAdded();
	v->fileActivated();

	
	if(folder)
	{
		DiskChange tmp(DiskChange::FILE_MODIFIED, folder, this);
		folder->getDisk()->onChange(tmp);
	}
}

/*
 * metoda, ktera nahodne vygeneruje jmeno pro dany soubor podle typu souboru
 */
void File::genName()
{
	 string tmp = "";
	 ifstream adjectives("res/files/pridavna");
	 ifstream names("res/files/jmena");

	static vector<string> adj;
	static vector<string> nam;

	if(!adjectives.is_open() || !names.is_open())
		cout<<"Soubor nejde otevrit"<<endl;

	//pokud je prazdny nactu hodnoty, jinak ne e
	 if(adj.empty())
		while(getline(adjectives, tmp))
			adj.push_back(tmp);


	if(nam.empty())
		while(getline(names, tmp))
			nam.push_back(tmp);


	name="";
	name+=adj[Engine::random(0, adj.size() - 1)];
	name+=" ";
	name+=nam[Engine::random(0, nam.size() - 1)];


	adjectives.close();
	names.close();

		return;
}
