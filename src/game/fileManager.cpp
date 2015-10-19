#include "fileManager.h"

#include <stdio.h>

#include "image.h"
#include "text.h"
#include "rectangle.h"
#include "disk.h"

FileManager::FileManager(int width, int height)
{
	addChild(background = new Rectangle(width, height, Color(100, 100, 100)));
	addChild(fileGroup = new Group());

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

	topFilePos = 0;

	background->setOnScrollFunc(
		[this](int sc)
		{
			if(sc > 0)
			{
				topFilePos -= 1;
				if(topFilePos < 0) topFilePos = 0;
			}
			else
			{
				topFilePos += 1;
				// jak to tady sikovne omezit? je to potreba?
			}

			display(folder);

			return true;
		}
	);

	contextMenu = new Group();
}

FileManager::~FileManager()
{
	clearView();

	delete fileGroup;
	delete background;

	contextMenu->removeChildren(true);
	delete contextMenu;
}

void FileManager::display(Folder * folder)
{
	if(!folder) return;

	clearView();
	this->folder = folder;

	Text * txt = new Text(folder->getName() + (folder->isCompressed() ? " (komprimovaná složka)" : ""));
	fileGroup->addChild(txt);

	if(folder->getDisk()->isReadOnly()) background->setColor(Color(150, 100, 100));
	else background->setColor(Color(100, 100, 100));

	double y = txt->getHeight();
	int pos = 0;

	if(topFilePos)
	{
		Image * arrow = new Image("res/arrowUp.png");
		fileGroup->addChild(arrow);
		arrow->setPosition(Vec2(background->getWidth() - arrow->getWidth(), 0));
	}

	for(auto & f : folder->getFiles())
	{
		if(pos++ < topFilePos) continue;

		Image * img = NULL;
		if(f->getType() == File::TEXT) img = new Image("res/file.png");
		if(f->getType() == File::EXECUTABLE) img = new Image("res/exe.png");
		if(f->getType() == File::IMAGE) img = new Image("res/img.png");
		if(f->getType() == File::VIDEO) img = new Image("res/vid.png");

		img->setPosition(Vec2(10, y));
		fileGroup->addChild(img);

		char buf[128];
		if(f->isCompressed())
			sprintf(buf, "%-40s%-25s%s/%s",
				f->getName().c_str(),
				parsePermissions(f->getPermissions()).c_str(),
				fileSizeToStr(f->getSize()).c_str(),
				fileSizeToStr(f->getRealSize()).c_str());
		else
			sprintf(buf, "%-40s%-25s%s",
				f->getName().c_str(),
				parsePermissions(f->getPermissions()).c_str(),
				fileSizeToStr(f->getSize()).c_str());

		File * cutFile = clipboard.valid ? getFileFromPath(clipboard) : NULL;
		Text * txt = new Text(buf, 1.0, f == cutFile ? Color(150, 150, 150) : (f->getVirusState().virus && f->getVirusState().discovered ? Color(255, 100, 100) : Color(255, 255, 255)));
		txt->setPosition(Vec2(img->getWidth() * 2, y + 5));
		fileGroup->addChild(txt);

		txt->setOnClickFunc(
			[this, f](int x, int y, bool right)
			{
				selected = f;

				if(right)
				{
					showContextMenu();
					contextMenu->setPosition(Vec2(x, y) - this->getPosition());
				}
				else if(onClickFunc) onClickFunc(selected);

				return true;
			}
		);

		y += img->getHeight();

		if(y > background->getHeight() - img->getHeight())
		{
			Image * arrow = new Image("res/arrowDown.png");
			fileGroup->addChild(arrow);
			arrow->setPosition(Vec2(background->getWidth() - arrow->getWidth(), background->getHeight() - arrow->getHeight()));
			break;
		}
	}
}

void FileManager::clearView()
{
	selected = NULL;
	folder = NULL;

	fileGroup->removeChildren(true);
}

void FileManager::showContextMenu()
{
	int x = 0;
	int y = 0;

	if(!selected && (!clipboard.valid || (folder && folder->getDisk()->isReadOnly()))) return;

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
			return false;
		}
	);

	if(folder && folder->getDisk()->isReadOnly())
	{
		if(selected)
		{
			Text * copy = new Text("Kopírovat soubor");
			if(copy->getWidth() > x) x = copy->getWidth();
			y += copy->getHeight();
			contextMenu->addChild(copy);

			copy->setOnClickFunc(
				[this](int x, int y, bool right)
				{
					if(selected && selected->getFolder())
					{
						clipboard = getFilePath(selected);
						this->copy = true;
					}

					hideContextMenu();
					return true;
				}
			);
		}
	}
	else
	{
		if(selected)
		{
			Text * cut = new Text("Vyjmout soubor");
			if(cut->getWidth() > x) x = cut->getWidth();
			y += cut->getHeight();
			contextMenu->addChild(cut);

			cut->setOnClickFunc(
				[this](int x, int y, bool right)
				{
					if(selected && selected->getFolder())
					{
						clipboard = getFilePath(selected);
						copy = false;
						display(folder);
					}

					hideContextMenu();
					return true;
				}
			);
		}

		if(clipboard.valid)
		{
			Text * paste = new Text("Vložit soubor");
			paste->setPosition(Vec2(0, y));
			if(paste->getWidth() > x) x = paste->getWidth();
			y += paste->getHeight();
			contextMenu->addChild(paste);

			paste->setOnClickFunc(
				[this](int x, int y, bool right)
				{
					if(clipboard.valid)
					{
						File * f = getFileFromPath(clipboard);
						Folder * old = f->getFolder();

						if(copy)
						{
							f = new File(*f);

							if(folder->addFile(f))
							{
								clipboard.valid = false;
							}
							else delete f;
						}
						else if(old->removeFile(f->getName()))
						{
							if(!folder->addFile(f))
								old->addFile(f);
							else
								clipboard.valid = false;
						}
					}

					hideContextMenu();
					return true;
				}
			);
		}

		if(selected)
		{
			Text * remove = new Text("Smazat soubor");
			remove->setPosition(Vec2(0, y));
			if(remove->getWidth() > x) x = remove->getWidth();
			y += remove->getHeight();
			contextMenu->addChild(remove);

			remove->setOnClickFunc(
				[this](int x, int y, bool right)
				{
					if(selected && selected->getFolder())
					{
						Folder * f = selected->getFolder();
						File * cutFile = clipboard.valid ? getFileFromPath(clipboard) : NULL;

						if(cutFile == selected)
							clipboard.valid = false;

						f->removeFile(selected->getName());
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

void FileManager::hideContextMenu()
{
	Engine::setInputRoot(NULL);
	removeChild(contextMenu);
	selected = NULL;
}

string FileManager::parsePermissions(int permissions)
{
	char u = permissions % 1000 / 100;
	char g = permissions % 100 / 10;
	char o = permissions % 10;

	char buf[32];
	sprintf(buf, "%c%c%c %c%c%c %c%c%c (%03d)", u & 4 ? 'r' : '-', u & 2 ? 'w' : '-', u & 1 ? 'x' : '-'
											 , g & 4 ? 'r' : '-', g & 2 ? 'w' : '-', g & 1 ? 'x' : '-'
											 , o & 4 ? 'r' : '-', o & 2 ? 'w' : '-', o & 1 ? 'x' : '-'
											 , permissions);

	return buf;
}
