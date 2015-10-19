#include "text.h"

#include <SDL2/SDL_ttf.h>

#include <iostream>
using namespace std;

#include "window.h"

Text::Text(const string & text, double size, const Color & color, int maxWidth) : text(text), size(size), color(color), maxWidth(maxWidth)
{
	texture = NULL;
	refresh();
}

Text::~Text()
{
	if(texture) SDL_DestroyTexture(texture);
}

void Text::setSize(double size)
{
	this->size = size;
	refresh();
}

void Text::setColor(const Color & color)
{
	this->color = color;
	refresh();
}

void Text::setText(const string & text)
{
	this->text = text;
	refresh();
}

void Text::refresh()
{
	if(texture)
	{
		SDL_DestroyTexture(texture);
		texture = NULL;
	}

	// Using resolution independent font size, should be 14px on 600px resolution
	TTF_Font * font = TTF_OpenFont("res/font.ttf", size * Window::getHeight() / 42.0);
	SDL_Color textColor = { color.getR(), color.getG(), color.getB(), color.getA() };

	// Blended should be nicer than Solid
	SDL_Surface * surface;
	if(maxWidth) surface = TTF_RenderUTF8_Blended_Wrapped(font, text.size() ? text.c_str() : " ", textColor, maxWidth);
	else surface = TTF_RenderUTF8_Blended(font, text.size() ? text.c_str() : " ", textColor);

	if(!surface) cout << "Error while creating text." << endl;
	else
	{
		texture = SDL_CreateTextureFromSurface(Window::getRenderer(), surface);
		width = surface->w;
		height = surface->h;

		SDL_FreeSurface(surface);
	}

	TTF_CloseFont(font);
}

void Text::render()
{
	if(!visible) return;

	if(texture)
	{
		Vec2 p = getCombinedPosition();

		SDL_Rect destination;
		destination.x = round(p.getX());
		destination.y = round(p.getY());
		destination.w = round(width);
		destination.h = round(height);

		SDL_RenderCopy(Window::getRenderer(), texture, NULL, &destination);
	}
}

// test if x,y is inside the text
bool Text::isAt(double x, double y)
{
	if(!visible || !enabled) return false;

	Vec2 p = getCombinedPosition();

	return x >= p.getX() && x <= p.getX() + width
		&& y >= p.getY() && y <= p.getY() + height;
}
