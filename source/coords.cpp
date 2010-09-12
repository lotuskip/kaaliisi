#include "coords.h"

#include <cmath>
#include <algorithm>

namespace
{
using namespace std;

const e_Dir opposites[] = {
D_S, // D_N
D_SW, // D_NE
D_W, // D_E
D_NW, // D_SE
D_N, // D_S
D_NE, // D_SW
D_E, // D_W
D_SE // D_NW
};

}

e_Dir& operator++(e_Dir& d) { return d = (D_N==d) ? D_NW : e_Dir(d-1); }

e_Dir operator!(const e_Dir d) { return opposites[d]; }


Coords::Coords(char nx, char ny) : x(nx), y(ny) { }

Coords::~Coords() {}


float Coords::dist_eucl(const Coords &from_where) const
{
	return sqrt(pow(double(x - from_where.x),2) + pow(double(y - from_where.y),2));
}


short Coords::dist_walk(const Coords &from_where) const
{
	return max(abs(x - from_where.x), abs(y - from_where.y));
}


// coordinates of neighboring location according to direction
Coords Coords::in(const e_Dir d) const
{
	Coords c(x,y);
	switch(d)
	{
		case D_N: c.y--; break;
		case D_NE: c.x++; c.y--; break;
		case D_E: c.x++; break;
		case D_SE: c.x++; c.y++; break;
		case D_S: c.y++; break;
		case D_SW: c.x--; c.y++; break;
		case D_W: c.x--; break;
		case D_NW: c.x--; c.y--; break;
	}
	return c;
}


Coords& Coords::operator=(const Coords &b)
{
	x = b.x;
	y = b.y;
	return *this;
}


Coords Coords::operator+(const Coords &b) const
{
	Coords tmp(x + b.x, y + b.y);
	return tmp;
}


Coords Coords::operator-(const Coords &b) const
{
	Coords tmp(x - b.x, y - b.y);
	return tmp;
}


void Coords::operator+=(const Coords &b)
{
	x += b.x;
	y += b.y;
}


bool Coords::operator==(const Coords &b) const
{
	return (x == b.x && y == b.y);
}


e_Dir Coords::dir_to(Coords ref) const
{
	// Basically, we construct the vector from *this to ref and
	// shrink it (in a Euclidian fashion) to a unit vector, which
	// we then interpret as a direction:
	ref.x -= x;
	ref.y -= y;
	short len = short(ref.dist_eucl(Coords(0,0)));
	ref.x /= len;
	ref.y /= len;
	switch(ref.x + ref.y)
	{
	case 2: return D_SE;
	case -2: return D_NW;
	case 1: if(ref.x) return D_E;
		return D_S;
	case -1: if(ref.x) return D_W;
		return D_N;
	case 0: if(ref.x == 1) return D_NE;
		return D_SW;
	}
	return D_N; // reaching this line is an error!
}

