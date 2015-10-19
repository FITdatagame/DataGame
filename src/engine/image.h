/**
* @file image.h
*/
/* Draws a clickable bitmap.
 */

#ifndef IMAGE_H
#define IMAGE_H

#include <map>
using namespace std;

#include "clickable.h"

/**
 * @brief Třída reprezentuje bitmapový obrázek, který je možné umístit to grafu scény.
 */
class Image : public Clickable
{
public:
	Image(const string & filename, int width = 0, int height = 0);
	~Image();

	virtual void render();
	virtual bool isAt(double x, double y);

	int getWidth() const { return width; }
	int getHeight() const { return height; }

	static void freeCache();

private:
	SDL_Texture * texture;
	int width;
	int height;

	string filename;
	static map<string, SDL_Texture*> imageDB;
};

#endif // IMAGE_H
