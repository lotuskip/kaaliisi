#include "game.h"
#include "rng.h"
#include "monster.h"
#include "map.h"
#include "stuff.h"

#include <list>
#include <set>
#include <cmath>

// global RNGs
M_RNG m_draw;
B_RNG b_draw;
N_RNG n_draw;

Map curmap;
std::list<Monster> monsters;

extern std::string killer;
extern int killcount;

namespace
{
using namespace std;
using namespace IO;

const unsigned char MAX_MONSTERS = 65;

list<Item> items;
set<char> mon_types_killed;

//chances of generation: ("one in" chance, i.e. chances are 1/25, 1/31)
const char monstergen_one_in = 25;
const char itemgen_one_in = 31;

const Coords upperleft(0,0);


char input_to_dir(const char input)
{
	switch(input)
	{
	case '1': case 'b': return D_SW;
	case '2': case 'j': return D_S;
	case '3': case 'n': return D_SE;
	case '4': case 'h': return D_W;
	case '6': case 'l': return D_E;
	case '7': case 'y': return D_NW;
	case '8': case 'k': return D_N;
	case '9': case 'u': return D_NE;
	}
	return -1; // no direction
}


// For health printing: (find the closest rational number with 1-digit numerator&denominator)
char health_n = 1, health_m = 1;
const char testlist_n[26] = { 8, 7, 6, 5, 4, 7, 3, 5, 2, 5, 3, 4, 5, 1, 4, 2, 3, 1, 2, 3, 1, 2, 1, 1, 1, 1 };
const char testlist_m[26] = { 9, 8, 7, 6, 5, 9, 4, 7, 3, 8, 5, 7, 9, 2, 9, 5, 7, 3, 7, 8, 4, 9, 5, 6, 7, 8 };
void update_health_bar()
{
	int p = monsters.front().getE();
	int q = 100*monsters.front().getB();
	if(p == q)
	{
		health_n = health_m = 1; // highest possibility
		return;
	}
	//else
	for(int j = 0; j < 26; j++)
	{
		if(testlist_m[j]*p - testlist_n[j]*q > 0)
		{
			health_n = testlist_n[j];
			health_m = testlist_m[j];
			return;
		}
	}
	health_n = 1; // lowest possibility
	health_m = 9;
}


bool gen_monster() // returns true if PC dies
{
	Monster m;
	Coords c;
	m.init(monsters.front().getB());
	curmap.get_entry_pos(m.get_fig_ref(), monsters.front().get_pos());

	for(list<Monster>::iterator i = monsters.begin();
		i != monsters.end(); ++i)
	{
		if(i->get_fig_ref()->hits(*(m.get_fig_ref())))
		{
			if(i == monsters.begin())
			{
				// This shouldn't happen any more.
				killer = m.get_name();
				print_msg("You are taken by surprise by " + art(killer) + '!');
				return true;
			}
			// else
			print_msg("A " + i->get_name() + " is trampled!");
			monsters.erase(i);
			break;
		}
	}
	monsters.push_back(m);
	return false;
}


void gen_item()
{
	Coords c(m_draw.index(screen_x), m_draw.index(map_y));
	curmap.dig(c);
	Item i;
	i.init(c);
	items.push_back(i);
}


bool run_AI() // returns true if PC died
{
	if(monsters.size() > 1)
	{
		Coords pcc = monsters.front().get_pos();
		char d;
		list<Monster>::iterator mon_it = monsters.begin();
		++mon_it; // skip PC
		while(mon_it != monsters.end())
		{
			// very basic: if the PC is close, move towards him; otherwise move randomly. If dist to PC is 1, hit 'im
			if((d = mon_it->get_dist_to(pcc)) > 10)
				mon_it->move_randomly();
			else if(d == 1)
			{
				if(monsters.front().decrE(mon_it->getB()*(n_draw.index(5)+1)))
				{
					killer = mon_it->get_name();
					print_msg("You are put to death by the " + killer + ". Press any key.");
					return true;
				}
				//else
				print_msg("The " + mon_it->get_name() + " hits you.");
				mon_it->clear_stationary();
				update_health_bar();
				++mon_it;
				continue;
			}
			else
				mon_it->move_towards(pcc);
			// Moved (or didn't if would've left map)
			++mon_it;
		}
	}
	return false;
}

}


void Kaaliisi::init()
{
	Monster pc;
	pc.init(0);
	monsters.push_back(pc);
	curmap.init(screen_x,map_y);
	Coords PCcoords(screen_x/2, map_y/2);
	curmap.dig(PCcoords);
	monsters.front().set_pos(PCcoords);
}


char Kaaliisi::run()
{
	char input = 0;
	char c, b;
	list<Monster>::iterator mon_it;
	list<Item>::iterator ite_it;

	gen_monster();

	do {
		/// Let all monsters except PC act
		if(run_AI())
		{
			getkey(); // above returning true means PC died
			break;
		}

		/// generate more monsters:
		if(monsters.size() < MAX_MONSTERS && b_draw.one_in(monstergen_one_in) && gen_monster())
		{
			getkey(); // returning true means PC is read
			break;
		}

		/// generate more items, and check for picking them up
		if(b_draw.one_in(itemgen_one_in))
			gen_item();
		ite_it = items.begin();
		while(ite_it != items.end())
		{
			c = 0;
			for(mon_it = monsters.begin(); mon_it != monsters.end(); ++mon_it)
			{
				if(ite_it->test_pickup(&(*mon_it)))
				{
					ite_it = items.erase(ite_it);
					c = 1;
					if(mon_it == monsters.begin())
						update_health_bar(); // PC picked up stuff; might change stats
					break;
				}
			}
			if(!c)
				++ite_it;
		}

		/// Draw everything
		clr();
		curmap.draw();
		for(ite_it = items.begin(); ite_it != items.end(); ++ite_it)
			(*ite_it).draw();
		for(mon_it = monsters.begin(); mon_it != monsters.end(); ++mon_it)
			(*mon_it).get_fig_ref()->draw();
		cursor(upperleft);
		colour(C_WHITE);
		print(health_n+'0'); print('/'); print(health_m+'0');
		cursor(monsters.front().get_pos());

		/// Get player input
		if((c = input_to_dir((input = getkey()))) != -1)
		{
			if((b = monsters.front().trymove(e_Dir(c))) == 2) // hit a monster!
			{
				Figure tmp = *(monsters.front().get_fig_ref());
				tmp.set_pos(tmp.get_pos().in(e_Dir(c)));

				for(mon_it = ++(monsters.begin()); mon_it != monsters.end(); ++mon_it)
				{
					if(tmp.hits(*(mon_it->get_fig_ref())))
					{
						if(mon_it->decrE(monsters.front().getB()*(n_draw.index(5)+1)))
						{
							print_msg("You hit the " + (*mon_it).get_name() + " turning it into plasma.");
							++killcount;
							// Killing a monster that is at least as buff as you, or of new type gives B++:
							if(mon_it->getB() >= monsters.front().getB()
								|| !mon_types_killed.count(mon_it->getB()))
							{
								monsters.front().incrB(1);
								update_health_bar();
							}
							mon_types_killed.insert(mon_it->getB());
							monsters.erase(mon_it);
						}
						else
							print_msg("You hit the " + mon_it->get_name() + '.');
						break;
					}
				}
			}
			else if(b == -1) // moved out of map
			{
				// disallow if have neighbours:
				for(mon_it = ++(monsters.begin()); mon_it != monsters.end(); ++mon_it)
				{
					if(mon_it->get_dist_to(monsters.front().get_pos()) == 1)
					{
						print_msg("You cannot retreat while next to monsters!");
						goto cont_loop;
					}
				}
				if(monsters.front().getB() >= 50)
					break; // won!
				//else
				return 2; // player fled
			}
			else if(b == 0) // hit a wall
				print_msg("Mmpphh!");
			// else walked, okay
		}
		else if(input == '5' || input == '.')
		{
			print_msg("You rest.");
			monsters.front().rest();
			update_health_bar();
		}
		else if(input == 'i')
			Item::print_stuff();
		cont_loop:;
	} while(input != 'Q');

	if(input == 'Q')
		return 0;
	if(killer != "") // was killed!
		return -1;
	// else won!
	return 1;
}

