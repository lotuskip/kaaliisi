#include "figure.h"

Figure::Figure() {}
Figure::~Figure() {}


char Figure::symbol(const char x, const char y) const
{
	return symbols.at(y*xsize + x);
}

Coords Figure::get_pos() const { return pos; }
void Figure::set_pos(const Coords &c) { pos = c; }

char Figure::get_xsize() const { return xsize; }
char Figure::get_ysize() const { return symbols.size()/xsize; }

bool Figure::hits(const Figure &f) const
{
	Coords fpos = f.get_pos();

	char xdist = pos.x - fpos.x;
	char ydist = pos.y - fpos.y;
	if((xdist > xsize && xdist > f.get_xsize())
	|| (ydist > get_ysize() && ydist > f.get_ysize()))
		return false;

	char xx, yy;
	for(xx = 0; xx < xsize; xx++)
	{
		for(yy = 0; yy < get_ysize(); yy++)
		{
			if(xx+xdist < f.get_xsize() && xx+xdist >= 0 && yy+ydist < f.get_ysize() && yy+ydist >= 0
			&& symbol(xx, yy) != ' ' && f.symbol(xx+xdist,yy+ydist) != ' ')
				return true;
		}
	}
	return false;
}


bool Figure::hits(const Coords &c) const
{
	char xdist = pos.x - c.x;
	char ydist = pos.y - c.y;
	if((xdist > xsize && xdist > 1)
	|| (ydist > get_ysize() && ydist > 1))
		return false;

	char xx, yy;
	for(xx = 0; xx < xsize; xx++)
	{
		for(yy = 0; yy < get_ysize(); yy++)
		{
			if(xx+xdist < 1 && xx+xdist >= 0 && yy+ydist < 1 && yy+ydist >= 0
			&& symbol(xx, yy) != ' ')
				return true;
		}
	}
	return false;
}


void Figure::draw() const
{
	char x, y;
	IO::colour(colour);
	for(y = 0; y < get_ysize(); y++)
	{
		for(x = 0; x < xsize; x++)
		{
			if(symbols[y*xsize + x] != ' ')
			{
				IO::cursor(pos + Coords(x,y));
				IO::print(symbols[y*xsize + x]);
			}
		}
	}
}


void Figure::init_app(const std::string &symbs, const char _xsize, IO::e_Colour col)
{
	symbols.clear();
	colour = col;
	xsize = _xsize;

	for(unsigned char i = 0; i < symbs.size(); i++)
		symbols.push_back(symbs[i]);
}


bool Figure::on_map() const
{
	using namespace IO;
	return (pos.x >= 0 && pos.y >= 0 && pos.x + xsize - 1 < screen_x && pos.y + get_ysize() - 1 < map_y);
}

