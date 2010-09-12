#ifndef COORDS_H
#define COORDS_H

enum e_Dir { D_N = 0, D_NE, D_E, D_SE, D_S, D_SW, D_W, D_NW };
e_Dir& operator++(e_Dir& d); // "turns direction 1 click (45deg) counter-clockwise"
e_Dir operator!(const e_Dir d); // gives opposite direction


class Coords
{
public:
	Coords(char nx = 0, char ny = 0);
	~Coords();

	char x,y;
	float dist_eucl(const Coords &from_where) const; // euclidian distance
	short dist_walk(const Coords &from_where) const; // step dist

	// directions interpreted as neighboring coordinates
	Coords in(const e_Dir d) const;
	e_Dir dir_to(Coords ref) const; // which way to move to get to b?

	Coords& operator=(const Coords &b);
	Coords operator+(const Coords &b) const;
	Coords operator-(const Coords &b) const;
	void operator+=(const Coords &b);

	bool operator==(const Coords &b) const;
};

#endif
