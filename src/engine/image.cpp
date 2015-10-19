#include "image.h"

#include <iostream>
using namespace std;

#include <SDL2/SDL_image.h>

#ifndef NOGFX
#include <SDL2/SDL2_rotozoom.h>
#endif

#include "window.h"

map<string, SDL_Texture*> Image::imageDB;

Image::Image(const string & filename, int width, int height) : filename(filename)
{
	texture = NULL;

	auto it = imageDB.find(filename);
	if(it != imageDB.end())
	{
		texture = it->second;
		SDL_QueryTexture(texture, NULL, NULL, &this->width, &this->height);
	}
	else
	{
		cout << "load new texture" << endl;
		SDL_Surface * surface = IMG_Load(filename.c_str());
		if(!surface) cout << "Error while loading " << filename << endl;
		else
		{
#ifndef NOGFX
			if(width && height)
			{
				double zoomx = width / (double)surface->w;
				double zoomy = height / (double)surface->h;
				zoomSurface(surface, zoomx, zoomy, 1);

				this->width = width;
				this->height = height;
			}
			else
#endif
			{
				this->width = surface->w;
				this->height = surface->h;
			}

			texture = SDL_CreateTextureFromSurface(Window::getRenderer(), surface);
			SDL_FreeSurface(surface);

			imageDB[filename] = texture;
		}
	}
}

Image::~Image()
{
	if(texture)
	{
		if(imageDB.find(filename) == imageDB.end())
			SDL_DestroyTexture(texture);
	}
}

void Image::freeCache()
{
	for(auto & p : imageDB)
		SDL_DestroyTexture(p.second);

	imageDB.clear();
}

void Image::render()
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
bool Image::isAt(double x, double y)
{
	if(!visible || !enabled) return false;

	Vec2 p = getCombinedPosition();

	return x >= p.getX() && x <= p.getX() + width
		&& y >= p.getY() && y <= p.getY() + height;
}
