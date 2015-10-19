#include "game.h"

#include "window.h"

/**
 * @brief V konstruktoru vytváříme všechny herní objekty a lambda funkce,
 * které budou volané, pokud nastanou určité události.
 */
Game::Game()
{
	cout << "Game has started." << endl;

	// vytvoříme dva disky, jeden pro data uživatele a jeden na zálohy
	userDisk = new Disk(1E12, "Uživatel"); // 1 TB
	backupDisk = new Disk(1E12, "Zálohy", true); // 1TB

	// na uživatelském disku vytvoříme základní adresářovou strukturu
	Folder * down, * img, * exe, * vid, * doc;
	userDisk->getRootFolder()->addFolder((down = new Folder("Stažené")));
	userDisk->getRootFolder()->addFolder((doc = new Folder("Dokumenty")));
	userDisk->getRootFolder()->addFolder((img = new Folder("Obrázky")));
	userDisk->getRootFolder()->addFolder((vid = new Folder("Videa")));
	userDisk->getRootFolder()->addFolder((exe = new Folder("Programy")));

	// nastavíme vlajky základním složkám (znemožníme jejich smazání a přejmenování)
	down->unsetFlag(Folder::CAN_BE_COMPRESSED | Folder::CAN_BE_REMOVED | Folder::CAN_BE_RENAMED);
	doc->unsetFlag(Folder::CAN_BE_REMOVED | Folder::CAN_BE_RENAMED);
	img->unsetFlag(Folder::CAN_BE_REMOVED | Folder::CAN_BE_RENAMED);
	vid->unsetFlag(Folder::CAN_BE_REMOVED | Folder::CAN_BE_RENAMED);
	exe->unsetFlag(Folder::CAN_BE_REMOVED | Folder::CAN_BE_RENAMED);

	// vytvoříme průzkumník složek a nastavíme mu velikost přes půl obrazovky
	// pak si v něm necháme zobrazit kořenový adresář na uživatelském disku
	// a celý objekt vložíme do grafu scény
	folderManager = new FolderManager(Window::getWidth() / 2.0, Window::getHeight() - 100);
	folderManager->display(userDisk->getRootFolder());
	addChild(folderManager);

	// nastavíme, co se má stát po kliknutí na nějakou složku
	folderManager->setOnClickFunc(
		[this](Folder * folder)
		{
			// necháme si zobrazit obsah vybrané složky v průzkumníku souborů
			fileManager->display(folder);
		}
	);

	// vytvoříme průzkumník souborů a nastavíme mu velikost přes půl obrazovky
	// pak ho posuneme o půl obrazovky doprava (vedle průzkumníku souborů),
	// necháme ho zobrazit složku Stažené a nakonec ho přidáme do grafu scény
	fileManager = new FileManager(Window::getWidth() / 2.0, Window::getHeight() - 100);
	fileManager->setPosition(Vec2(Window::getWidth() / 2.0, 0));
	fileManager->display(down);
	addChild(fileManager);

	// nastavíme, co se má stát po kliknutí na soubor
	fileManager->setOnClickFunc(
		[this](File * file)
		{
			// je-li soubor na read-only disku (zálohovacím), neuděláme nic
			if(file->getFolder()->getDisk()->isReadOnly()) return;

			// vypneme průzkumník složek a zobrazíme volič práv
			folderManager->setEnabled(false);
			selector->setVisible(true);

			selector->openFile(file);
		}
	);

	// vytvoříme volič práv, posuneme ho do prostřed obrazovky a zatím ho schováme
	selector = new PermissionSelector();
	selector->setPosition(Vec2(Window::getWidth() / 2.0 - 600 / 2.0, Window::getHeight() / 2.0 - 384 / 2.0));
	selector->setVisible(false);
	addChild(selector);

	// nastavíme, co se má stát po upravení práv
	selector->setOnClickFunc(
		[this](File * file)
		{
			// schováme volič a zapneme průzkumník složek
			selector->setVisible(false);
			folderManager->setEnabled(true);

			// obnovíme pohled na složku s průzkumníku souborů
			fileManager->display(file->getFolder());
		}
	);

	// vytvoříme běžící text a umístíme ho dolů
	movingText = new MovingTextManager();
	movingText->setPosition(Vec2(0, Window::getHeight() - 50));
	addChild(movingText);

	// přidáme tlačítko pro zobrazení zálohovacího disku a nastavíme,
	// co se má stát po kliknutí na něj
	addChild(backupDiskButton = new Text("Procházet zálohy"));
	backupDiskButton->setPosition(Vec2(10, Window::getHeight() - 100));
	backupDiskButton->setOnClickFunc(
		[this](int x, int y, bool right)
		{
			folderManager->display(backupDisk->getRootFolder());
			fileManager->display(backupDisk->getRootFolder());
			backupShown = true;
			return true;
		}
	);

	// přidáme tlačítko pro zobrazení uživatelského disku a nastavíme,
	// co se má stát po kliknutí na něj
	addChild(userDiskButton = new Text("Procházet uživatelský disk"));
	userDiskButton->setPosition(Vec2(backupDiskButton->getWidth() + 30, Window::getHeight() - 100));
	userDiskButton->setOnClickFunc(
		[this, down](int x, int y, bool right)
		{
			folderManager->display(userDisk->getRootFolder());
			fileManager->display(down);
			backupShown = false;
			return true;
		}
	);

	// přidáme tlačítko pro zobrazení uživatelského disku a nastavíme,
	// co se má stát po kliknutí na něj
	addChild(backupButton = new Text("Provést zálohu"));
	backupButton->setPosition(Vec2(backupDiskButton->getWidth() + userDiskButton->getWidth() + 60, Window::getHeight() - 100));
	backupButton->setOnClickFunc(
		[this](int x, int y, bool right)
		{
			bool res = userDisk->backup(backupDisk);
			if(backupShown) folderManager->display(backupDisk->getRootFolder());
			pause();

			MessageWindow * msg = new MessageWindow();
			addChild(msg);
			if(res)
				msg->setMessage("Všechny soubory byly zálohovány. Zálohy můžeš procházet stisknutím tlačítka 'Procházet zálohy'.");
			else
				msg->setMessage("Záloha nemohla být vytvořena, neboť není na disku dostatek volného místa.");
			msg->addOption("Rozumím.", 0);
			msg->setOnClickFunc([this, msg](int code){ if(code == 0){ msg->hide(); removeChild(msg); run(); delete msg; } });
			msg->show();

			return true;
		}
	);

	// přidáme tlačítko pro zobrazení seznamu úkolů a nastavíme,
	// co se má stát po kliknutí na něj
	addChild(tasksButton = new Text("Seznam úkolů"));
	tasksButton->setPosition(Vec2(backupButton->getWidth() + backupDiskButton->getWidth() + userDiskButton->getWidth() + 90, Window::getHeight() - 100));
	tasksButton->setOnClickFunc(
		[this](int x, int y, bool right)
		{
			taskManager->show();

			return true;
		}
	);

	// přidáme tlačítko pro zobrazení správce firewallu a nastavíme,
	// co se má stát po kliknutí na něj
	addChild(firewallButton = new Text("Firewall"));
	firewallButton->setPosition(Vec2(backupButton->getWidth() + backupDiskButton->getWidth() + userDiskButton->getWidth() + tasksButton->getWidth() + 120, Window::getHeight() - 100));
	firewallButton->setOnClickFunc(
		[this](int x, int y, bool right)
		{
			firewallManager->show();

			return true;
		}
	);

	// přidáme tlačítko pro spuštění antiviru a nastavíme,
	// co se má stát po kliknutí na něj
	addChild(antivirusButton = new Text("Antivirus"));
	antivirusButton->setPosition(Vec2(backupButton->getWidth() + backupDiskButton->getWidth() + userDiskButton->getWidth() + tasksButton->getWidth() + firewallButton->getWidth() + 150, Window::getHeight() - 100));
	antivirusButton->setOnClickFunc(
		[this](int x, int y, bool right)
		{
			// nastavíme, co se má stát po dokončení skenu
			antivirusManager->setOnScanComplete(
				[this](int res)
				{
					// vypočteme, kolik času dáme hráči na zlikvidování virů
					antivirusFreezeTimeout = res * 5.0;
					pause();

					char buf[512];
					sprintf(buf, "Sken byl dokončen a nalezl %d infekcí. Po přečtení zpávy máš %d sekund na to, abys smazal napadené soubory, "
								 "než se znovu aktivují v nich obsažené viry.", res, (int)antivirusFreezeTimeout);

					MessageWindow * msg = new MessageWindow();
					addChild(msg);
					msg->setMessage(buf);
					msg->addOption("Rozumím.", 0);
					msg->setOnClickFunc([this, msg](int code){ if(code == 0){ msg->hide(); removeChild(msg); run(); delete msg; } });
					msg->show();

					return true;
				}
			);

			// spustíme antivirus na kořenový adresář
			antivirusManager->processFolder(userDisk->getRootFolder());

			return true;
		}
	);

	// vytvoříme správce firewallu
	firewallManager = new FirewallManager();
	addChild(firewallManager);

	firewallManager->addPort(Port(80, "HTTP"));
	firewallManager->addPort(Port(22, "SSH"));
	firewallManager->addPort(Port(21, "FTP"));
	firewallManager->addPort(Port(25, "SMTP"));
	firewallManager->addPort(Port(143, "IMAP"));
	firewallManager->addPort(Port(5900, "VNC"));
	firewallManager->addPort(Port(25565, "Minecraft"));

	// vytvoříme správce úkolů
	taskManager = new TaskManager(down, img, doc, exe, vid, firewallManager);
	addChild(taskManager);

	taskManager->setOnNewFile(
		[this](File * f)
		{
			movingText->addText("Byl stažen nový soubor '" + f->getName() + "'");
		}
	);

	taskManager->setOnNewTask(
		[this](Task * t)
		{
			movingText->addText("Máš nový úkol '" + t->getName() + "'");
		}
	);

	taskManager->setOnTaskFinished(
		[this](Task * t)
		{
			movingText->addText("Úkol '" + t->getName() + "' byl dokončen " + (t->isDone() ? "úspěšně" : "neúspěšně"));
		}
	);

	// vytvoříme správce antiviru
	antivirusManager = new AntivirusManager();
	addChild(antivirusManager);

	// hodíme si sem uvítací okno se zprávou
	welcomeMessage = new MessageWindow();
	addChild(welcomeMessage);
	welcomeMessage->setMessage("Vítej ve hře o bezpečnosti při práci s daty na počítači. Tvým úkolem je starat se o nezodpovědného uživatele, "
							"který nedbá na pořádek v počítači. Budeš po něm muset třídit stažené soubory do vhodných složek, "
							"čas od času zálohovat, protože tvůj uživatel tu a tam ztratí nějaký soubor a chce ho zpátky. Některé stažené "
							"soubory obsahují i virus, takže až zpozoruješ záhadně mizející, přibývající a měnící se soubory, je na čase "
							"spustit antivirus. Uživatel tě může někdy požádat o otevření portů ve firewallu, ale dej si pozor! Jakmile už "
							"uživatel port nepotřebuje zase ho zavři, aby toho někdo nezneužil. Dívej se také do seznamu aktuálních úkolů, "
							"protože se tam dozvíš, jaké úkoly máš splnit, kolik za ně dostaneš bodů a kolik můžeš ztratit, pokud je nesplníš.");
	welcomeMessage->addOption("Jasně, jde se na to!", 0);
	welcomeMessage->setOnClickFunc(
		[this](int code)
		{
			if(code == 0)
			{
				welcomeMessage->hide();
				run();
			}
		}
	);
	welcomeMessage->show();

	pauseText = new Text("HRA JE POZASTAVENA", 1.2, Color(255, 100, 100));
	pauseText->setPosition(Vec2(Window::getWidth() - pauseText->getWidth() - 10, 10));
	addChild(pauseText);
	pauseText->setVisible(false);

	// NA KONEC vytvoříme funkci, která se zavolá po každé změně v souborech a složkách
	userDisk->setOnChange(
		[this](DiskChange c)
		{
			//pokud je disk uplne zaplnen, zobrazime, ze nastal konec hry
			if(c.fullDisk == true)
			{
				pause();

				// hodíme si sem ukončovací okno se zprávou
				MessageWindow * finalMessage = new MessageWindow();
				addChild(finalMessage);
				finalMessage->setMessage("Konec hry! Tvůj uživatelský disk byl úplně zaplněn. Výsledné skóre bude uloženo.");
				finalMessage->addOption("Ne, už to skončilo?!", 0);
				finalMessage->setOnClickFunc(
					[this, finalMessage](int code)
					{
						if(code == 0)
						{
							Engine::stop();
							removeChild(finalMessage);
							delete finalMessage;
						}
					}
				);
				finalMessage->show();
			}

			if(!backupShown)
			{
				redrawRequired = true;

				// pokud byla smazána složka, kterou máme otevřenou, tak vyčistíme průzkumník souborů
				if(c.type == c.FOLDER_REMOVED && fileManager->getFolder() == c.folder)
					fileManager->clearView();
			}

			// informujeme o změně správce úkolů
			taskManager->diskChange(c);
		}
	);

	// podobnou funkci vytvoříme i pro zálohovací disk
	backupDisk->setOnChange(
		[this](DiskChange c)
		{
			if(backupShown)
			{
				redrawRequired = true;

				// pokud byla smazána složka, kterou máme otevřenou, tak vyčistíme průzkumník souborů
				if(c.type == c.FOLDER_REMOVED && fileManager->getFolder() == c.folder)
					fileManager->clearView();
			}
		}
	);

	pause();
}

Game::~Game()
{
	delete userDisk;
	delete backupDiskButton;
	delete backupDisk;
	delete folderManager;
	delete fileManager;
	delete selector;
	delete movingText;
	delete backupButton;
	delete userDiskButton;
	delete welcomeMessage;
	delete taskManager;
	delete tasksButton;
	delete firewallManager;
	delete firewallButton;
	delete antivirusButton;
	delete antivirusManager;
	delete pauseText;

	cout << "Game has finished." << endl;
}

void Game::update()
{
	if(isRunning)
	{
		antivirusFreezeTimeout -= Engine::getDelta();

		vector<Virus*> & viruses = taskManager->getViruses();
		auto it = viruses.begin();
		while(it != viruses.end())
		{
			if((*it)->isAlive())
			{
				if(antivirusFreezeTimeout < 1E-9)
					(*it)->update(userDisk->getRootFolder());
				++it;
			}
			else
			{
				delete *it;
				it = viruses.erase(it);
			}
		}

		taskManager->taskUpdate();
	}

	if(redrawRequired)
	{
		redrawRequired = false;
		folderManager->display(folderManager->getFolder());
		fileManager->display(fileManager->getFolder());
	}

	Group::update();
}
