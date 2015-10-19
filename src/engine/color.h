/**
* @file color.h
*/
/* Color holds 32bit color information and can do linear interpolation between two colors.
 */

#ifndef COLOR_H
#define COLOR_H

/**
 * @brief Třída reprezentuje RGBA barvu s 8 bity na kanál.
 */
class Color {
public:
	Color(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255) :
		r(r), g(g), b(b), a(a) {}

	/**
	 * @brief Provede lineární interpolaci mezi dvěma barvami.
	 * @param c
	 * @param p
	 * @return
	 */
	Color lerp(const Color & c, double p) const { return Color(r + (c.r - r) * p, g + (c.g - g) * p, b + (c.b - b) * p, a + (c.a - a) * p); }

	void setR(unsigned char r) { this->r = r; }
	unsigned char getR() const { return r; }

	void setG(unsigned char g) { this->g = g; }
	unsigned char getG() const { return g; }

	void setB(unsigned char b) { this->b = b; }
	unsigned char getB() const { return b; }

	void setA(unsigned char a) { this->a = a; }
	unsigned char getA() const { return a; }

private:
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

#endif // COLOR_H
