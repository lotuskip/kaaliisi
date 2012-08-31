#ifndef STUFF_H
#define STUFF_H

#include <map>
#include <string>

#include "coords.h"
#include "io.h"

class Monster;

const char NUM_OTHERS = 50;

class Item
{
public:
	Item();
	~Item();

	void init(const Coords loc); // generates a random item
	void draw() const;
	bool test_pickup(Monster* bywhom) const; // does whatever happens when this item is picked up, after which caller will delete this object

	static void print_stuff();
	static short get_inventory_size();

private:
	char symbol;
	IO::e_Colour col;
	char food_index; // 0..NUM_FOODS for foods; -1 for others
	std::string name;
	Coords location;

	static std::map<std::string,short> inventory;
};

#endif
