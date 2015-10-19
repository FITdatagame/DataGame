#include "permissionSelector.h"

#include <stdio.h>

PermissionSelector::PermissionSelector()
{
	vals[0] = vals[1] = vals[2] = 0;

	addChild(table = new Image("res/permissionTable.png"));
	addChild(buttons[0] = new Image("res/permissionButtonUp.png"));
	addChild(buttons[1] = new Image("res/permissionButtonUp.png"));
	addChild(buttons[2] = new Image("res/permissionButtonUp.png"));
	addChild(buttons[3] = new Image("res/permissionButtonDown.png"));
	addChild(buttons[4] = new Image("res/permissionButtonDown.png"));
	addChild(buttons[5] = new Image("res/permissionButtonDown.png"));
	addChild(ok = new Image("res/permissionButtonOK.png"));
	addChild(values[0] = new Text("0", 5, Color(0, 0, 0, 255)));
	addChild(values[1] = new Text("0", 5, Color(0, 0, 0, 255)));
	addChild(values[2] = new Text("0", 5, Color(0, 0, 0, 255)));

	buttons[0]->setPosition(Vec2(75, 30));
	buttons[1]->setPosition(Vec2(205, 30));
	buttons[2]->setPosition(Vec2(340, 30));
	buttons[3]->setPosition(Vec2(75, 290));
	buttons[4]->setPosition(Vec2(205, 290));
	buttons[5]->setPosition(Vec2(340, 290));

	ok->setPosition(Vec2(475, 165));

	values[0]->setPosition(Vec2(105, 125));
	values[1]->setPosition(Vec2(238, 125));
	values[2]->setPosition(Vec2(370, 125));

	for(int i = 0; i < 6; i++)
		buttons[i]->setOnClickFunc([this, i](int x, int y, bool right){ changeValue(i, i < 3); return false; });

	ok->setOnClickFunc([this](int x, int y, bool right)
	{
		int permissions = vals[0] * 100 + vals[1] * 10 + vals[2];
		file->setPermissions(permissions);
		if(onClickFunc) onClickFunc(file);

		return true;
	});
}

PermissionSelector::~PermissionSelector()
{
	removeChildren(true);
}

void PermissionSelector::openFile(File * file)
{
	this->file = file;
	int permissions = file->getPermissions();

	vals[0] = permissions % 1000 / 100;
	vals[1] = permissions % 100 / 10;
	vals[2] = permissions % 10;

	char buf[4];

	sprintf(buf, "%d", vals[0]);
	values[0]->setText(buf);
	sprintf(buf, "%d", vals[1]);
	values[1]->setText(buf);
	sprintf(buf, "%d", vals[2]);
	values[2]->setText(buf);
}

void PermissionSelector::changeValue(int pos, bool increase)
{
	if(increase) vals[pos % 3]++;
	else vals[pos % 3]--;

	if(vals[pos % 3] > 7) vals[pos % 3] = 0;
	if(vals[pos % 3] < 0) vals[pos % 3] = 7;

	char buf[4];
	sprintf(buf, "%d", vals[pos % 3]);

	values[pos % 3]->setText(buf);
}
