#include "vec2.h"

ostream & operator<<(ostream & os, const Vec2 & v)
{
	return os << "(" << v.x << ", " << v.y << ")";
}

Vec2 operator*(double d, const Vec2 & v)
{
	return Vec2(v) * d;
}
