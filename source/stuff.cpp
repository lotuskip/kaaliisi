#include "stuff.h"
#include "rng.h"
#include "monster.h"
#include <sstream>

namespace
{
using namespace std;
using namespace IO;

const char NUM_FOODS = 7;

const char chance_food = 62;

const string food_names[NUM_FOODS] = { "grand slam", "pearly jam", "creaky clam",
"mouldy ham", "taco sham", "can of spam" /* stolen from Alphaman */, "leg of lamb" };

const e_Colour food_colours[NUM_FOODS] = { C_LIGHT_GREEN, C_WHITE, C_GREEN,
C_LIGHT_RED, C_YELLOW, C_LIGHT_GRAY, C_BROWN };

// Many of these have been inspired by such games as ADOM, Nethack, and Alphaman
const string other_names[NUM_OTHERS] = { "sharp rock", "plastic cloak", "studded girdle",
"mithril cowboy hat", "buffy ring", "son-of-a-bitch sword", "boon amulet", "macroscopic shield",
"Beginner's Kit", "Pink Bible", "voodoo troll", "zapping wand", "bribe", "detonator",
"pretty pendant", "answer sheet", "fountain pen", "small round object", "mood ring",
"moody ring", "malloch armour", "leafsucker", "plastic wrap", "regular symbol",
"vol. I of the Magic Mini Giant", "all-important thingamabob", "Elbereth sticker",
"umbrella", "inscribing kit", "herbal kit", "large round object", "multi-purpose rod",
"whizzy staff", "great filthy rag", "cybernephrotic implant", "extra finger",
"chaos reward", "hidden flower", "super-hero cape", "balalaika", "box set",
"quest item", "vol. II of the Magic Mini Giant", "strange item", "force field generator",
"Universal Gear", "solution", "loom", "vol. III of the Magic Mini Giant", "Grail" };

string lex_cast(const int n)
{
	stringstream ss;
	ss << n;
	return ss.str();
}

} // end local namespace

map<string,short> Item::inventory;


Item::Item() : symbol('='), food_index(-1) { }
Item::~Item() { }


void Item::init(const Coords loc)
{
	location = loc;
	if(m_draw.index(100) < chance_food)
	{
		food_index = m_draw.index(NUM_FOODS);
		name = food_names[food_index];
		symbol = '0';
		col = food_colours[food_index];
	}
	else
	{
		name = other_names[n_draw.index(NUM_OTHERS)];
		col = e_Colour(m_draw.index(MAX_COLOR));
	}
}


void Item::draw() const
{
	cursor(location);
	colour(col);
	print(symbol);
}


bool Item::test_pickup(Monster* bywhom) const
{
	if(bywhom->get_fig_ref()->hits(location))
	{
		if(food_index == -1) // random item
		{
			if(bywhom->get_name() == "you") // only collect stuff if picked up by the PC!
			{
				inventory[name]++;
				string msg = "You grab ";
				// every *new* item increases Buffness:
				if(inventory[name] == 1)
				{
					bywhom->incrB(1);
					msg += art(name);
				}
				else
					msg += "another " + name;
				msg += '.';
				print_msg(msg);
			}
		}
		else
			bywhom->eat(name, food_index);
		return true;
	}
	return false;
}


void Item::print_stuff() // static!
{
	string s = "You are carrying: ";
	if(inventory.size())
	{
		for(map<string,short>::const_iterator i = inventory.begin(); i != inventory.end(); ++i)
		{
			if(i->second > 1)
				s += lex_cast(i->second) + ' ' + i->first + 's';
			else s += art(i->first);
			s += ", ";

		}
		s.erase(s.size()-2);
		s += '.';
	}
	else s += "nothing.";
	clr_print(s);
}


short Item::get_inventory_size() { return inventory.size(); }

