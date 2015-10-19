#include "inputDialog.h"

#include "rectangle.h"
#include "utils.h"
#include "window.h"

InputDialog::InputDialog()
{
	int width = 300;
	int height = 100;
	int maxChars = 15;

	addChild(new Rectangle(width, height, Color(50, 50, 50, 255)));
	setPosition(Vec2((Window::getWidth() / 2.0 - width) / 2.0, (Window::getHeight() / 2.0 - height) / 2.0));

	Text * desc = new Text("Zadej název složky a stiskni Enter.");
	addChild(desc);
	desc->setPosition(Vec2(10, 10));

	text = new Text("");
	addChild(text);
	text->setPosition(Vec2(10, 40));

	text->setOnTextFunc(
		[this, maxChars](const char * txt)
		{
			wstring wide;
			utf8toWStr(wide, text->getText());

			if((int)wide.length() < maxChars)
				text->setText(text->getText() + txt);

			return false;
		}
	);

	text->setKeyDownFunc(
		[this](const SDL_Scancode & scancode)
		{
			if(scancode == SDL_SCANCODE_BACKSPACE)
			{
				string narrow = text->getText();
				wstring wide;
				utf8toWStr(wide, narrow);

				wide = wide.substr(0, wide.length() - 1);

				wstrToUtf8(narrow, wide);
				text->setText(narrow);
			}
			else if(scancode == SDL_SCANCODE_RETURN)
			{
				if(text->getText().size() && onDone) onDone(text->getText());
			}

			return true;
		}
	);

	Engine::setInputRoot(this);
}


InputDialog::~InputDialog()
{
	removeChildren(true);
	Engine::setInputRoot(NULL);
}
