#include "folderManager.h"

#include <iostream>
using namespace std;

#include "rectangle.h"
#include "inputDialog.h"
#include "disk.h"
#include "utils.h"

FolderManager::FolderManager(int width, int height)
{
	addChild(background = new Rectangle(width, height, Color(100, 100, 100)));
	addChild(folderGroup = new Group());

	background->setOnClickFunc(
		[this](int x, int y, bool right)
		{
			if(right)
			{
				showContextMenu();
				contextMenu->setPosition(Vec2(x, y) - this->getPosition());
			}

			return true;
		}
	);

	diskState = new Text("Zaplnění disku: 0%");
	addChild(diskState);
	diskState->setPosition(Vec2(10, height - diskState->getHeight()));

	topFolderPos = 0;
	background->setOnScrollFunc(
		[this](int sc)
		{
			if(sc > 0)
			{
				topFolderPos -= 1;
				if(topFolderPos < 0) topFolderPos = 0;
			}
			else
			{
				topFolderPos += 1;
				// jak to tady sikovne omezit? je to potreba?
			}

			display(rootFolder);

			return true;
		}
	);

	contextMenu = new Group();
}

FolderManager::~FolderManager()
{
	contextMenu->removeChildren(true);
	delete contextMenu;

	folderGroup->removeChildren(true);

	delete folderGroup;
	delete background;
	delete diskState;
}

void FolderManager::display(Folder * rootFolder)
{
	this->rootFolder = rootFolder;
	prepareDisplay = true;
}

void FolderManager::doDisplay()
{
	prepareDisplay = false;

	vector<pair<Folder*, int>> folders;
	rootFolder->getAllFoldersRecursively(folders, false);

	folderGroup->removeChildren(true);

	if(rootFolder->getDisk()->isReadOnly()) background->setColor(Color(150, 100, 100));
	else background->setColor(Color(100, 100, 100));

	int y = 0;
	int pos = 0;

	if(topFolderPos)
	{
		Image * arrow = new Image("res/arrowUp.png");
		folderGroup->addChild(arrow);
		arrow->setPosition(Vec2(background->getWidth() - arrow->getWidth(), 0));
	}

	for(auto & p : folders)
	{
		if(pos++ < topFolderPos) continue;

		Folder * f = p.first;

		Image * i = f->isCollapsed() ? new Image("res/folder.png") : new Image("res/folderOpen.png");
		i->setPosition(Vec2(i->getWidth() * p.second, y));
		i->setOnClickFunc([this, f](int x, int y, bool right) { f->setCollapsed(!f->isCollapsed()); display(rootFolder); return false; } );
		folderGroup->addChild(i);

		if(f->isCompressed())
		{
			Image * zip = new Image("res/zip.png");
			zip->setPosition(i->getPosition());
			folderGroup->addChild(zip);
		}

		Text * t = new Text(f->getName());
		t->setPosition(Vec2(i->getWidth() * (p.second + 1), y));
		t->setOnClickFunc(
			[this, f](int x, int y, bool right)
			{
				selected = f;

				if(right)
				{
					showContextMenu();
					contextMenu->setPosition(Vec2(x, y));
				}
				else if(onClickFunc) onClickFunc(selected);

				return true;
			}
		);
		folderGroup->addChild(t);

		y += i->getHeight();
		if(y > background->getHeight() - i->getHeight() - diskState->getHeight())
		{
			Image * arrow = new Image("res/arrowDown.png");
			folderGroup->addChild(arrow);
			arrow->setPosition(Vec2(background->getWidth() - arrow->getWidth(), background->getHeight() - arrow->getHeight()));
			break;
		}
	}

	long long int sz = rootFolder->getSize();
	long long int cap = rootFolder->getDisk()->getCapacity();
	int percFull = 100 * sz / cap;

	char buf[128];
	sprintf(buf, "Zaplnění disku: %s z %s (%d%%)", fileSizeToStr(sz).c_str(), fileSizeToStr(cap).c_str(), percFull);

	diskState->setText(buf);
	if(percFull > 90) diskState->setColor(Color(255, 0, 0, 255));
	else diskState->setColor(Color(255, 255, 255, 255));
}

void FolderManager::update()
{
	if(prepareDisplay) doDisplay();

	Group::update();
}

void FolderManager::showContextMenu()
{
	int x = 0;
	int y = 0;

	if(rootFolder && rootFolder->getDisk()->isReadOnly()) return;
	if(!selected) return;

	contextMenu->removeChildren(true);
	Engine::setInputRoot(contextMenu);
	addChild(contextMenu);

	Rectangle * background = new Rectangle(0, 0, Color(50, 50, 50, 255));
	contextMenu->addChild(background);

	background->ignoreEventPosition(true);
	background->setOnClickFunc(
		[this](int x, int y, bool right)
		{
			hideContextMenu();
			return true;
		}
	);

	if(selected)
	{
		Text * create = new Text("Vytvořit složku");
		if(create->getWidth() > x) x = create->getWidth();
		y += create->getHeight();
		contextMenu->addChild(create);

		create->setOnClickFunc(
			[this](int x, int y, bool right)
			{
				Engine::setInputRoot(NULL);
				removeChild(contextMenu);

				if(selected)
				{
					InputDialog * inputDialog = new InputDialog();

					inputDialog->setDoneFunc(
						[this, inputDialog](const string & result)
						{
							Folder * f = new Folder(result);
							selected->addFolder(f);
							selected = NULL;
							removeChild(inputDialog);
							delete inputDialog;
						}
					);

					addChild(inputDialog);
				}

				return true;
			}
		);

		if(selected->getFlag(Folder::CAN_BE_RENAMED))
		{
			Text * rename = new Text("Přejmenovat složku");
			rename->setPosition(Vec2(0, y));
			if(rename->getWidth() > x) x = rename->getWidth();
			y += rename->getHeight();
			contextMenu->addChild(rename);

			rename->setOnClickFunc(
				[this](int x, int y, bool right)
				{
					Engine::setInputRoot(NULL);
					removeChild(contextMenu);

					if(selected)
					{
						InputDialog * inputDialog = new InputDialog();

						inputDialog->setDoneFunc(
							[this, inputDialog](const string & result)
							{
								selected->setName(result);
								selected = NULL;
								removeChild(inputDialog);
								delete inputDialog;
							}
						);

						addChild(inputDialog);
					}

					return true;
				}
			);
		}

		if(selected->getFlag(Folder::CAN_BE_COMPRESSED))
		{
			Text * compress = new Text(selected->isCompressed() ? "Dekomprimovat složku" : "Komprimovat složku");
			compress->setPosition(Vec2(0, y));
			if(compress->getWidth() > x) x = compress->getWidth();
			y += compress->getHeight();
			contextMenu->addChild(compress);

			compress->setOnClickFunc(
				[this](int x, int y, bool right)
				{
					if(selected)
						selected->setCompressed(!selected->isCompressed());

					hideContextMenu();
					return true;
				}
			);
		}

		if(selected->getFlag(Folder::CAN_BE_REMOVED))
		{
			Text * remove = new Text("Smazat složku");
			remove->setPosition(Vec2(0, y));
			if(remove->getWidth() > x) x = remove->getWidth();
			y += remove->getHeight();
			contextMenu->addChild(remove);

			remove->setOnClickFunc(
				[this](int x, int y, bool right)
				{
					if(selected && selected->getParent() && selected->getName() != "Stažené")
					{
						selected->getParent()->removeFolder(selected->getName());
						delete selected;
						selected = NULL;
					}

					hideContextMenu();
					return true;
				}
			);
		}
	}

	background->setWidth(x);
	background->setHeight(y);
}

void FolderManager::hideContextMenu()
{
	Engine::setInputRoot(NULL);
	removeChild(contextMenu);
	selected = NULL;
}
