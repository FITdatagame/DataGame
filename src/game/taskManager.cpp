#include "taskManager.h"

#include "rectangle.h"
#include "window.h"
#include "utils.h"

int Task::taskIdGenerator = 0;

TaskManager::TaskManager(Folder * dwnFolder, Folder * imgFolder, Folder * docFolder, Folder * exeFolder, Folder * vidFolder, FirewallManager * firewallManager) :
	dwnFolder(dwnFolder), imgFolder(imgFolder), docFolder(docFolder), exeFolder(exeFolder), vidFolder(vidFolder), firewallManager(firewallManager)
{
	tasksGroup = new Group();
	elements = new Group();

	Rectangle * background = new Rectangle(Window::getWidth(), Window::getHeight(), Color(50, 50, 50, 255));
	elements->addChild(background);

	Text * text = new Text("Zde je seznam všech vašich aktuálních úkolů. Kromě těchto úkolů nezapoměňte přesouvat nově "
						   "stažené soubory ze složky Stažené do jiných vhodných složek.", 1.0, Color(255, 255, 255), Window::getWidth() / 2);
	elements->addChild(text);

	filesTask = new Text("Aktuálně je potřeba roztřídit 0 souborů.", 1.0, Color(200, 200, 200));
	elements->addChild(filesTask);
	filesTask->setPosition(Vec2(0, text->getHeight() + 5));

	Text * back = new Text("Zavřít seznam", 1.0, Color(255, 100, 100));
	elements->addChild(back);
	back->setPosition(Vec2(Window::getWidth() - back->getWidth() - 10, 0));

	back->setOnClickFunc(
		[this](int x, int y, bool right)
		{
			hide();
			return true;
		}
	);

	scoreText = new Text("Skóre: 00000");
	addChild(scoreText);
	scoreText->setPosition(Vec2(Window::getWidth() - scoreText->getWidth() - 10, Window::getHeight() - 100));

	nextFileDownload = 5.0;
	nextTaskTimeout = 7.0;
	nextListRefresh = 1.0;
	nextFirewallCheck = 3.0;
}

TaskManager::~TaskManager()
{
	GameResult::appendResult(gameResult, "results.bin");

	tasksGroup->removeChildren(true);
	elements->removeChildren(true);

	for(auto & v : viruses) delete v;

	if(firewallTask) delete firewallTask;
	if(antivirusTask) delete antivirusTask;

	delete tasksGroup;
	delete elements;
	delete scoreText;
}

void TaskManager::show()
{
	addChild(elements);
	addChild(tasksGroup);
	Engine::setInputRoot(this);
}

void TaskManager::hide()
{
	removeChild(elements);
	removeChild(tasksGroup);
	Engine::setInputRoot(NULL);
}

void TaskManager::taskUpdate()
{
	// je-li vhodny cas, stahneme novy soubor
	nextFileDownload -= Engine::getDelta();
	if(nextFileDownload <= 0)
	{
		nextFileDownload = Engine::random(10000, 20000) / 1000.0;

		File * newFile = new File();
		dwnFolder->addFile(newFile);

		if(Engine::random(0, 1000000) > 900000)
		{
			Virus * newVirus = new Virus();
			newFile->setVirus(newVirus);
			viruses.push_back(newVirus);

			if(!antivirusTask)
			{
				AntivirusTask * t = new AntivirusTask();
				t->name = "Najdi a zlikviduj všechny viry v systému.";
				t->description = "Použij antivirus a následně smaž všechny nakažené soubory.";
				t->pointsGain = ANTIVIRUS_TASK_BONUS;
				t->pointsLose = 0;
				t->timeLeft = NO_TIME_LIMIT;

				antivirusTask = t;
				tasks[t->id] = t;

				if(onNewTask) onNewTask(t);
			}
			else antivirusTask->inc();
		}

		if(onNewFile) onNewFile(newFile);
	}

	// je-li vhodny cas, vygenerujeme novy task
	nextTaskTimeout -= Engine::getDelta();
	if(nextTaskTimeout <= 0)
	{
		nextTaskTimeout = Engine::random(10000, 20000) / 1000.0;

		int nextTask = Engine::random(10000, 20000) % 2;

		if(deletedData.size() && nextTask == 0)
		{
			int i = Engine::random(0, (int)deletedData.size() - 1);

			UndeleteTask * t = new UndeleteTask(dwnFolder->getDisk()->getRootFolder(), deletedData[i].second);
			t->name = "Obnov ze zálohy smazaný soubor '" + deletedData[i].first + "'.";
			t->description = "Pokud soubor v záloze nemáš, nedá se nic dělat.";
			t->pointsGain = UNDELETE_TASK_BONUS;
			t->pointsLose = UNDELETE_TASK_PENALTY;
			t->timeLeft = 45.0;

			deletedData.erase(deletedData.begin() + i);

			if(onNewTask) onNewTask(t);

			undeleteTasks.push_back(t);
			tasks[t->id] = t;
		}
		else if(!firewallTask && nextTask == 1)
		{
			Port p = firewallManager->getRandomClosedPort();

			if(p.number > 0)
			{
				char buf[64];
				sprintf(buf, "Otevři port číslo %d.", p.number);

				FirewallTask * t = new FirewallTask(dwnFolder->getDisk()->getRootFolder(), p.number);
				t->name = buf;
				t->description = "Až tento úkol splníš, můžeš port zase zavřít, aby ho někdo nezneužil.";
				t->pointsGain = FIREWALL_TASK_BONUS;
				t->pointsLose = FIREWALL_TASK_PENALTY;
				t->timeLeft = 25.0;

				firewallTask = t;
				tasks[t->id] = t;

				if(onNewTask) onNewTask(t);
			}
		}
	}

	// zkontrolujeme undelete tasky
	for(auto it = undeleteTasks.begin(); it != undeleteTasks.end(); it++)
		if(checkTask((*it)))
		{
			if(onTaskFinished) onTaskFinished((*it));

			if((*it)->isDone()) gameResult.numUndeleteCorrect++;
			else gameResult.numUndeleteIncorrect++;

			tasks.erase((*it)->id);
			delete (*it);
			undeleteTasks.erase(it);
			break;
		}

	// zkontrolujeme firewall task
	if(firewallTask)
	{
		firewallTask->check(firewallManager);

		if(checkTask(firewallTask))
		{
			if(onTaskFinished) onTaskFinished(firewallTask);

			if(firewallTask->isDone()) gameResult.firewallTasksDone++;
			else gameResult.firewallTasksIgnored++;

			tasks.erase(firewallTask->id);
			delete firewallTask;
			firewallTask = NULL;
		}
	}

	// zkontrolujeme antivirus task
	if(antivirusTask)
	{
		antivirusTask->check(viruses);

		if(checkTask(antivirusTask))
		{
			if(onTaskFinished) onTaskFinished(antivirusTask);

			if(antivirusTask->isDone())
				gameResult.virusesDefeated += antivirusTask->cnt();

			tasks.erase(antivirusTask->id);
			delete antivirusTask;
			antivirusTask = NULL;
		}
	}

	// zkontrolujeme otevrene porty na firewallu a pripadne trochu zaskodime
	nextFirewallCheck -= Engine::getDelta();
	if(nextFirewallCheck <= 0)
	{
		nextFirewallCheck = Engine::random(10000, 20000) / 1000.0;
		firewallCheck();
	}

	// pravidelne obnovujeme seznam ukolu
	nextListRefresh -= Engine::getDelta();
	if(nextListRefresh <= 0)
	{
		nextListRefresh = 1.0;
		refresh();
	}
}

void TaskManager::diskChange(const DiskChange & c)
{
	int badFiles = 0, goodFiles = 0, badPermissions = 0;
	vector<File*> files;

	docFolder->getAllFilesRecursively(files);
	for(auto & f : files)
	{
		if(f->getType() == File::TEXT) goodFiles++;
		else badFiles++;

		if(!checkFilePermissions(f))
			badPermissions++;
	}
	files.clear();

	imgFolder->getAllFilesRecursively(files);
	for(auto & f : files)
	{
		if(f->getType() == File::IMAGE) goodFiles++;
		else badFiles++;

		if(!checkFilePermissions(f))
			badPermissions++;
	}
	files.clear();

	exeFolder->getAllFilesRecursively(files);
	for(auto & f : files)
	{
		if(f->getType() == File::EXECUTABLE) goodFiles++;
		else badFiles++;

		if(!checkFilePermissions(f))
			badPermissions++;
	}
	files.clear();

	vidFolder->getAllFilesRecursively(files);
	for(auto & f : files)
	{
		if(f->getType() == File::VIDEO) goodFiles++;
		else badFiles++;

		if(!checkFilePermissions(f))
			badPermissions++;
	}
	files.clear();

	char buf[512];
	sprintf(buf, "Aktuálně je potřeba roztřídit %d souborů. Správně jsi roztřídil %d souborů. Špatně jsi roztřídil %d souborů.",
			(int)dwnFolder->getFiles().size(),
			goodFiles,
			badFiles);

	gameResult.filesManagedCorrectly = goodFiles;
	gameResult.filesManagedIncorrectly = badFiles;
	gameResult.numWrongPermissions = badPermissions;

	filesTask->setText(buf);

	if(c.type == DiskChange::FILE_REMOVED)
		deletedData.push_back(pair<string, int>(c.removedFileName, c.removedFileData));

	if(c.type == DiskChange::FILE_ADDED)
	{
		for(auto & t : undeleteTasks)
			t->check(c);

		for(auto it = deletedData.begin(); it != deletedData.end(); it++)
			if((*it).second == c.file->getData())
			{
				deletedData.erase(it);
				break;
			}
	}
}

void TaskManager::refresh()
{
	char buf[512];
	double yPos = 100;
	int i = 1;

	tasksGroup->removeChildren(true);

	for(auto & p : tasks)
	{
		Task * t = p.second;

		sprintf(buf, "%d. %s", i++, t->name.c_str());
		Text * name = new Text(buf);
		name->setPosition(Vec2(10, yPos));
		tasksGroup->addChild(name);

		sprintf(buf, "Možný zisk: %d", t->pointsGain);
		Text * gain = new Text(buf);
		gain->setPosition(Vec2(560, yPos));
		tasksGroup->addChild(gain);

		sprintf(buf, "Možná ztráta: %d", t->pointsLose);
		Text * loss = new Text(buf);
		loss->setPosition(Vec2(710, yPos));
		tasksGroup->addChild(loss);

		if(t->timeLeft == NO_TIME_LIMIT)
			sprintf(buf, "Na úkol máš neomezený čas.");
		else
			sprintf(buf, "Zbývající čas: %d", (int)t->timeLeft);
		Text * left = new Text(buf);
		left->setPosition(Vec2(860, yPos));
		tasksGroup->addChild(left);

		Text * desc = new Text(t->description, 0.85);
		desc->setPosition(Vec2(23, yPos + 23));
		tasksGroup->addChild(desc);

		yPos += 60;
	}

	sprintf(buf, "Skóre: %05d", gameResult.calcScore());
	scoreText->setText(buf);
}

void TaskManager::firewallCheck()
{
	Port p = firewallManager->getRandomOpenPort();
	if(p.number == 0) return;

	// otevrene FTP, zaskodime v souborech
	if(p.number == 21)
	{
		Folder * srcFolder = NULL;
		Folder * destFolder = NULL;

		// vygenerujeme cisla pro nahodnou zdrojovou a cilovou slozku
		int srcRand = Engine::random(0, 10000) % 4;
		int destRand = Engine::random(0, 10000) % 4;

		// kdyz to padne na stejnou slozku, vybereme dalsi
		if(destRand == srcRand) destRand = (destRand + 1) % 4;

		// vybereme nahodne zdrojovou slozku
		if(srcRand == 0) srcFolder = imgFolder;
		else if(srcRand == 1) srcFolder = vidFolder;
		else if(srcRand == 2) srcFolder = exeFolder;
		else if(srcRand == 3) srcFolder = docFolder;

		// vybereme nahodne cilovou slozku
		if(destRand == 0) destFolder = imgFolder;
		else if(destRand == 1) destFolder = vidFolder;
		else if(destRand == 2) destFolder = exeFolder;
		else if(destRand == 3) destFolder = docFolder;

		// presuneme nahodny soubor ze zdrojove do cilove slozky :D
		File * f = findRandomFile(srcFolder);
		if(f)
		{
			srcFolder->removeFile(f->getName());
			destFolder->addFile(f);
		}

		gameResult.numFirewallBreaches++;
	}
}

bool TaskManager::checkFilePermissions(File * f)
{
	if(f->getType() == File::TEXT && f->getPermissions() != 644) return false;
	if(f->getType() == File::IMAGE && f->getPermissions() != 644) return false;
	if(f->getType() == File::VIDEO && f->getPermissions() != 644) return false;
	if(f->getType() == File::EXECUTABLE && f->getPermissions() != 755) return false;

	return true;
}

bool TaskManager::checkTask(Task * t)
{
	if(t->timeLeft < 0 && t->timeLeft > NO_TIME_LIMIT)
	{
		return true;
	}

	if(t->isDone())
	{
		return true;
	}

	if(t->timeLeft > NO_TIME_LIMIT)
		t->timeLeft -= Engine::getDelta();

	return false;
}

void UndeleteTask::check(const DiskChange & c)
{
	if(c.type == DiskChange::FILE_ADDED
	&& c.file->getData() == missingData)
		done = true;
}

void FirewallTask::check(FirewallManager * f)
{
	Port p = f->getPort(port);

	if(p.open) done = true;
}

void AntivirusTask::check(vector<Virus*> & viruses)
{
	if(viruses.size() == 0)
		done = true;
}
