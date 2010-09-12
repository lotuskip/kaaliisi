#ifndef FIGURE_H
#define FIGURE_H

#include <vector>

#include "coords.h"
#include "io.h"

// A "Figure" describes the position, dimensions (x- and y-size), and appearance of a monster
class Figure
{
public:
	Figure();
	~Figure();

	void init_app(const std::string &symbs, const char _xsize, IO::e_Colour col);
	void draw() const;

	char symbol(const char x, const char y) const; // NOTE: relative coords, ie. (0,0) is upper-left corner _of the figure_!

	bool hits(const Figure &f) const; // collision detection
	bool hits(const Coords &c) const;
	bool on_map() const; // returns false if is partially outside of map

	char get_xsize() const;
	char get_ysize() const;

	Coords get_pos() const;
	void set_pos(const Coords &c);

private:
	Coords pos; // position of upper-left corner (even if empty)
	char xsize; // ysize = symbols.size()/xsize
	std::vector<char> symbols;
	IO::e_Colour colour;
};

#endif
