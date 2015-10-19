#include "input.h"

#include <algorithm>

#include "engine.h"
#include "clickable.h"

Clickable * Input::selected = NULL;
int Input::mPosX = 0;
int Input::mPosY = 0;

// receive the events and call appropriate callbacks
void Input::update(Object * root)
{
	SDL_Event sdl_event;

	while(SDL_PollEvent(&sdl_event) > 0)
	{
		switch(sdl_event.type)
		{
		case SDL_QUIT: // system exit call - no handler can override that
			Engine::stop();
			break;
		case SDL_KEYDOWN: // handle key press
			if(sdl_event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				Engine::stop();

			root->input(
				[sdl_event](Clickable * c)
				{
					return c->onKeyDown(sdl_event.key.keysym.scancode);
				}
			);
			break;
		case SDL_KEYUP: // handle key release

			break;
		case SDL_TEXTINPUT:
			root->input(
				[sdl_event](Clickable * c)
				{
					return c->onText(sdl_event.text.text);
				}
			);
			break;

		// forward mouse events to clickable objects at current mouse position
		case SDL_MOUSEWHEEL:
			root->input(
				[sdl_event](Clickable * c)
				{
					if(c->getIgnoreEventPosition() || c->isAt(mPosX, mPosY))
						return c->onScroll(sdl_event.wheel.y);
					return false;
				}
			);
			break;
		case SDL_MOUSEBUTTONDOWN:
			root->input(
				[sdl_event](Clickable * c)
				{
					if(c->getIgnoreEventPosition() || c->isAt(sdl_event.button.x, sdl_event.button.y))
						return c->onClick(sdl_event.button.x, sdl_event.button.y, sdl_event.button.button == SDL_BUTTON_RIGHT);
					return false;
				}
			);
			break;
		case SDL_MOUSEBUTTONUP:
			root->input(
				[sdl_event](Clickable * c)
				{
					if(c->getIgnoreEventPosition() || c->isAt(sdl_event.button.x, sdl_event.button.y))
						return c->onRelease(sdl_event.button.x, sdl_event.button.y);
					return false;
				}
			);
			break;
		case SDL_MOUSEMOTION:
			mPosX = sdl_event.motion.x;
			mPosY = sdl_event.motion.y;

			root->input(
				[sdl_event](Clickable * c)
				{
					if(c->getIgnoreEventPosition() || c->isAt(sdl_event.motion.x, sdl_event.motion.y))
					{
						if(selected != c)
						{
							if(selected) selected->onOut(sdl_event.motion.x, sdl_event.motion.y);

							selected = c;
							selected->onIn(sdl_event.motion.x, sdl_event.motion.y);
						}

						c->onPoint(sdl_event.motion.x, sdl_event.motion.y);

						return true;
					}
					else if(c == selected)
					{
						selected->onOut(sdl_event.motion.x, sdl_event.motion.y);
						selected = NULL;
					}

					return false;
				}
			);
			break;
		default:
			break;
		}
	}
}

void Input::objectRemoved(Object * c)
{
	if(selected == c)
		selected = NULL;
}
