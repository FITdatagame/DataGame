#include "virus.h"

#include "engine.h"
#include "folder.h"
#include "file.h"
#include "utils.h"

Virus::Virus()
{
	type = (Type)(rand() % 3);
	active = false;
	nextHarm = Engine::random(10, 20); // 10 - 20 vteřin

	cout << "virus created " << (int)type << endl;
}

bool Virus::update(Folder * rootFolder)
{
	bool ret = false;

	if(nextHarm > 1E-9)
	{
		nextHarm -= Engine::getDelta();
		return ret;
	}

	if(active)
	{
		// do some harm
		cout << "harming..." << endl;

		switch(type)
		{
		// nebezpečí smazání souboru, který je ve schránce!!!
		case ERASING:
		{
			File * victim = findRandomFile(rootFolder);
			if(!victim || victim->getVirusState().virus == this) break;

			cout << "mažu soubor" << endl;

			victim->getFolder()->removeFile(victim->getName());
			delete victim;
			ret = true;

			cout << "virus erased some file" << endl;
			break;
		}
		case CREATING:
		{
			Folder * victim = findRandomFolder(rootFolder);
			if(!victim) break;

			File * f = new File();
			f->setVirus(this);
			while(!victim->addFile(f))
				f->setName(f->getName() + ((char)(65 + Engine::random(0, 26))));
			ret = true;

			cout << "virus created some file" << endl;
			break;
		}
		case MODIFYING:
		{
			File * victim = findRandomFile(rootFolder);
			if(!victim) break;

			if(victim->getVirusState().virus != this) victim->setVirus(this);
			victim->setPermissions(Engine::random(0, 7) + Engine::random(0, 7) * 10 + Engine::random(0, 7) * 100);
			ret = true;

			cout << "virus modified some file" << endl;
			break;
		}
		}

		nextHarm = Engine::random(10, 20);
	}

	return ret;
}
