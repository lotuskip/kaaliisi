#include "map.h"
#include "rng.h"
#include "io.h"

#include <cmath>

namespace
{
using namespace std;
}

Map::Map() : dimensions(Coords(0,0)) {}
Map::~Map() {}

void Map::init(const char xsize, const char ysize)
{
	char x, y;
	// clear any old map
	for(y = 0; y < dimensions.y; y++)
		walls[y].clear();
	walls.clear();

	// The following code is heavily influenced by the ant nest algorithm used in Xenocide; credits!
	for(y = 0; y < ysize; y++)
		walls.push_back(vector<bool>(xsize, true));
	walls[ysize/2][xsize/2] = false;
	dimensions.x = xsize;
	dimensions.y = ysize;

	float x1, y1;
	float k;
	float dx, dy;
	char px, py;
	int counter;

	m_draw.set_lim(100);
	for(short i = 0; i < short(xsize)*ysize/3; i++)
	{
		k = m_draw.fdraw()*3.14159/2;
		// position on ellipse by degree:
		x1 = xsize/2+(xsize/2)*sin(k);
		y1 = ysize/2+(ysize/2)*cos(k);

		// object will move not too horizontal and not too vertical
		do {
			dx = m_draw();
			dy = m_draw();
		} while(dx < 10 && dy < 10);
		dx -= 50;
		dy -= 50;
		dx /= 100;
		dy /= 100;

		counter = 0;
		for(;;)
		{
			// didn't catch anything after 1000 steps (just to avoid infinite loops)
			if(counter++ > 1000)
			{
				i--;
				break;
			}
			// move object by small step
			x1 += dx;
			y1 += dy;
			px = char(x1); // round
			py = char(y1);

			// go through the border to the other side
			if(px < 0)
			{
				px = xsize-1;
				x1 = px;
			}
			if(px > xsize-1)
			{
				px = 0;
				x1 = px;
			}
			if(py < 0)
			{
				py = ysize-1;
				y1 = py;
			}
			if(py > ysize-1)
			{
				py = 0;
				y1 = py;
			}

			// if object has something to catch, then catch it
			if((px > 0 && !walls[py][px-1]) ||
				(py > 0 && !walls[py-1][px]) ||
				(px < xsize-1 && !walls[py][px+1]) ||
				(py < ysize-1 && !walls[py+1][px]))
			{
				walls[py][px] = false;
				break;
			}
		}
	}

	// add halls at the end of corridors
	for(x = 1; x < xsize-1; x++)
	{
		for(y = 1; y < ysize-1; y++)
		{
			if((x > xsize/2-10 && x < xsize/2+10 && y > ysize/2-5 && y < ysize/2+5) || !walls[y][x])
				continue;

			counter = 0;
			if(!walls[y-1][x-1])
				counter++;
			if(!walls[y-1][x])
				counter++;
			if(!walls[y-1][x+1])
				counter++;
			if(!walls[y][x-1])
				counter++;
			if(!walls[y][x+1])
				counter++;
			if(!walls[y+1][x-1])
				counter++;
			if(!walls[y+1][x])
				counter++;
			if(!walls[y+1][x-1])
				counter++;

			if(counter == 1)
			{
				for(px = -1; px <= 1; px++)
				{
					for(py = -1; py <= 1; py++)
						walls[y+py][x+px] = false;
				}
			}
		}
	}
}


bool Map::collision(const Figure &f) const
{
	Coords pos = f.get_pos();
	char x,y;
	for(x = pos.x; x < f.get_xsize() + pos.x; x++)
	{
		for(y = pos.y; y < f.get_ysize() + pos.y; y++)
		{
			if(y < 0 || x < 0 // point outside of map
			|| y >= IO::map_y || x >= IO::screen_x)
				continue;
			if(walls[y][x] && f.symbol(x - pos.x, y - pos.y) != ' ')
				return true;
		}
	}
	return false;
}


void Map::draw() const
{
	using namespace IO;
	cursor(Coords(0,0));
	for(char y = 0; y < dimensions.y; y++)
	{
		for(char x = 0; x < dimensions.x; x++)
		{
			if(walls[y][x])
			{
				colour(C_DARK_GRAY);
				print('#');
			}
			else
			{
				colour(C_LIGHT_GRAY);
				print('.');
			}
		}
	}
}


void Map::dig(const Coords &c) { walls.at(c.y).at(c.x) = false; }


void Map::get_entry_pos(Figure* f, const Coords &avoid)
{
	// start randomly from either top or bottom
	Coords ret(dimensions.x/2,0);
	bool bottom = false;
	if((bottom = b_draw.one_in(2)))
		ret.y = dimensions.y - 1;
	ret.x += m_draw.pm(9);

	// (may assume monsters aren't that wide)
	//ret.x -= max(f->get_xsize() + ret.x - dimensions.x, 0);
	ret.y -= max(f->get_ysize() + ret.y - dimensions.y, 0);

	// Check that not too close to the coords to avoid (the PC)
	if(avoid.dist_walk(ret) < 2*max(f->get_ysize(), f->get_xsize()))
	{
		if(ret.x < dimensions.x/2)
			ret.x += 2*f->get_xsize();
		else
			ret.x -= 2*f->get_xsize();
	}

	f->set_pos(ret);
	char x,y;
	for(x = ret.x; x < f->get_xsize() + ret.x; ++x)
	{
		for(y = ret.y; y < f->get_ysize() + ret.y; ++y)
			dig(Coords(x,y));
	}
}

