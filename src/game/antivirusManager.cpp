#include "antivirusManager.h"

#include "engine.h"

AntivirusManager::~AntivirusManager()
{
	if(msg) delete msg;
}

void AntivirusManager::processFolder(Folder * folder)
{
	msg = new MessageWindow();
	addChild(msg);
	msg->setMessage("Prosím počkej, než se dokončí skenování...");
	msg->show();

	int num = 0;

	vector<File*> fls;
	folder->getAllFilesRecursively(fls);

	for(auto & f : fls)
	{
		if(f->getVirusState().virus)
		{
			f->getVirusState().discovered = true;
			num++;
		}
	}

	if(fls.size() < 10) scanTimeout = 5.0;
	else if(fls.size() < 100) scanTimeout = fls.size() * 0.1 + 5.0;
	else scanTimeout = 15.0;

	scanResult = num;
}

void AntivirusManager::update()
{
	if(scanTimeout > 0)
	{
		scanTimeout -= Engine::getDelta();

		if(scanTimeout <= 0)
		{
			msg->hide();
			removeChild(msg);
			delete msg;
			msg = NULL;

			if(onScanComplete) onScanComplete(scanResult);
		}
	}
}
