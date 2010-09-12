#ifndef MAP_H
#define MAP_H

#include <vector>

#include "figure.h"


class Map
{
public:
	Map();
	~Map();

	void init(const char xsize, const char ysize); // generates a random map
	void dig(const Coords &c); // force c to be open
	void draw() const;

	bool collision(const Figure &f) const; // checks if f hits any walls
	void get_entry_pos(Figure* f, const Coords &avoid); // puts the figure at a suitable location

private:
	Coords dimensions;
	std::vector< std::vector<bool> > walls; // perhaps a bit less sloppy memory-wise than bool[][]..
};

#endif
