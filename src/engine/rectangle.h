/**
* @file rectangle.h
*/
/* Draws a non-dynamic (does not change shape or color) clickable rectangle.
 * We allow the rectangle to be redrawn with new parameters but this will
 * regenerate the texture which is a slow process.
 */

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "clickable.h"
#include "color.h"

/**
 * @brief Reprezenuje obdélník, který lze přidat do grafu scény.
 */
class Rectangle : public Clickable
{
public:
	Rectangle(int width = 0, int height = 0, const Color & color = Color(255, 255, 255, 255));
	virtual ~Rectangle();

	virtual void render();
	virtual bool isAt(double x, double y);

	void setWidth(int width);
	int getWidth() const { return width; }

	void setHeight(int height);
	int getHeight() const { return height; }

	void setColor(const Color & color);
	Color getColor() const { return color; }

protected:
	void refresh();

	int width;
	int height;
	Color color;

	SDL_Texture * texture;
};

#endif // RECTANGLE_H
