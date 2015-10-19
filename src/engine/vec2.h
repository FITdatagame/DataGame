/**
* @file vec2.h
*/
/* Simple and compact class which represents two dimensional vector.
 * Supports all common operations like addition, subtraction, multiplication,
 * multiplication by constant, normalization, calculation of dot product and length.
 * Using operator overloading where possible.
 */

#ifndef VEC2_H
#define VEC2_H

#include <cmath>
#include <iostream>

using namespace std;

/**
 * @brief Třída reprezentuje dvoudimezionální vektor. Kromě uvedených metod má ještě přetížené všechny běžné operátory.
 */
class Vec2 {
public:
	Vec2(double x = 0, double y = 0) : x(x), y(y) {}
	Vec2(const Vec2 & r) { x = r.x; y = r.y; }
	~Vec2() {}

	bool operator==(const Vec2 & r) const { return (x - r.x)*(x - r.x) < 1E-12 && (y - r.y)*(y - r.y) < 1E-12; }
	bool operator!=(const Vec2 & r) const { return !(operator==(r)); }

	Vec2 & operator=(const Vec2 & r) { x = r.x; y = r.y; return *this; }

	Vec2 & operator+=(const Vec2 & r) { x += r.x; y += r.y; return *this; }
	Vec2 & operator-=(const Vec2 & r) { x -= r.x; y -= r.y; return *this; }
	Vec2 & operator*=(const Vec2 & r) { x *= r.x; y *= r.y; return *this; }
	Vec2 & operator/=(const Vec2 & r) { x /= r.x; y /= r.y; return *this; }
	Vec2 & operator*=(double r) { x *= r; y *= r; return *this; }

	Vec2 operator+(const Vec2 & r) const { return Vec2(*this) += r; }
	Vec2 operator-(const Vec2 & r) const { return Vec2(*this) -= r; }
	Vec2 operator*(const Vec2 & r) const { return Vec2(*this) *= r; }
	Vec2 operator/(const Vec2 & r) const { return Vec2(*this) /= r; }
	Vec2 operator*(double r) const { return Vec2(*this) *= r; }

	/**
	 * @brief Vrátí skalární součin dvou vektorů.
	 * @param r
	 * @return
	 */
	double dot(const Vec2 & r) const { return x * r.x + y * r.y; }

	/**
	 * @brief Vrátí velikost vektoru.
	 * @return
	 */
	double length() { return sqrt(dot(*this)); }

	/**
	 * @brief Normalizuje tento vektor.
	 * @return
	 */
	Vec2 & normalize() { return (*this *= 1.0/length()); }

	/**
	 * @brief Vrátí normalizovanou kopii vektoru.
	 * @return
	 */
	Vec2 normalized() const { return Vec2(*this).normalize(); }

	/**
	 * @brief Provede lineární interpolaci mezi dvěma vektory.
	 * @param r
	 * @param p
	 * @return
	 */
	Vec2 lerp(const Vec2 & r, double p) const { return *this + (r - *this) * p; }

	friend ostream & operator<<(ostream & os, const Vec2 & v);
	friend Vec2 operator*(double d, const Vec2 & v);

	void setX(double x) { this->x = x; }
	double getX() const { return x; }

	void setY(double y) { this->y = y; }
	double getY() const { return y; }

private:
	double x;
	double y;
};

#endif
