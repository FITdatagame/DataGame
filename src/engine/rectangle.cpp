#include "rectangle.h"

#include "window.h"

Rectangle::Rectangle(int width, int height, const Color & color) : width(width), height(height), color(color)
{
	texture = NULL;
	refresh();
}

Rectangle::~Rectangle()
{
	if(texture) SDL_DestroyTexture(texture);
}

void Rectangle::setWidth(int width)
{
	this->width = width;
	refresh();
}

void Rectangle::setHeight(int height)
{
	this->height = height;
	refresh();
}

void Rectangle::setColor(const Color & color)
{
	this->color = color;
	refresh();
}

void Rectangle::refresh()
{
	if(texture)
	{
		SDL_DestroyTexture(texture);
		texture = NULL;
	}

	SDL_Surface * surface = SDL_CreateRGBSurface(0, width, height, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	if(!surface)
	{
		texture = NULL;
		cout << "Error while creating rectangle." << endl;
		return;
	}

	SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, color.getR(), color.getG(), color.getB(), color.getA()));

	texture = SDL_CreateTextureFromSurface(Window::getRenderer(), surface);

	SDL_FreeSurface(surface);
}

void Rectangle::render()
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
bool Rectangle::isAt(double x, double y)
{
	if(!visible || !enabled) return false;

	Vec2 p = getCombinedPosition();

	return x >= p.getX() && x <= p.getX() + width
		&& y >= p.getY() && y <= p.getY() + height;
}
