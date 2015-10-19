/**
* @file text.h
*/
/* This class uses SDL_ttf to render a static text. */

#ifndef TEXT_H
#define TEXT_H

#include "clickable.h"
#include "color.h"

/**
 * @brief Reprezentuje text, který je možné přidat do grafu scény.
 */
class Text : public Clickable
{
public:
	Text(const string & text, double size = 1.0, const Color & color = Color(255, 255, 255, 255), int maxWidth = 0);
	virtual ~Text();

	virtual void render();
	virtual bool isAt(double x, double y);

	void setSize(double size);
	double getSize() const { return size; }

	void setColor(const Color & color);
	Color getColor() const { return color; }

	void setText(const string & text);
	string getText() const { return text; }

	int getWidth() const { return width; }
	int getHeight() const { return height; }

private:
	void refresh();

	string text;
	double size;
	Color color;
	int maxWidth;

	SDL_Texture * texture;
	int width;
	int height;
};

#endif // TEXT_H
