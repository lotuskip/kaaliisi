#include "monster.h"
#include "rng.h"
#include "map.h"

#include <list>
#include <vector>

extern std::list<Monster> monsters;

namespace
{
using namespace IO;
using namespace std;

const short MAX_MONSTER_B = 22;

const string apps[MAX_MONSTER_B] = {
"%", //lunchkin
"$", //double snake
"\"", //toam
"(*)", //poof
"}{", //giant batling
"^", //yardworm
"T", //anime tree
"__||", //pie monster [2x2]
"o//o", //big lunchkin [2x2]
"[=] I ", //chainsaw wielding space orc [3x2]
"||", //big toam
"/\\", //greater yardworm [2x1]
"k", //kinetic kid
"<>||<>", //malevolent abstraction [2x3]
"V'", //stalactite [1x2]
"||!!", //enormous toam [2x2]
"--- | ", //mega anime tree [3x2]
".", // invisible perv
" | -+- | ", //holy slayer [3x3]
"O / / / O", //super-size lunchkin [3x3]
" /\\ /  \\", //greatest yardworm [4x2]
" _/ L_" //chaos cactus [2x3]			B=21
};

const e_Colour colours[MAX_MONSTER_B] = {
C_BROWN, //lunchkin
C_GREEN, //double snake
C_YELLOW, //toam
C_LIGHT_GRAY, //poof
C_DARK_GRAY, //giant batling
C_MAGENTA, //yardworm
C_BROWN, //anime tree
C_BROWN, //pie monster
C_BROWN, //big lunchkin
C_RED, //chainsaw wielding space orc
C_YELLOW, //big toam
C_MAGENTA, //greater yardworm
C_LIGHT_CYAN, //kinetic kid
C_CYAN, //malevolent abstraction
C_LIGHT_GRAY, //stalactite
C_YELLOW, //enormous toam
C_BROWN, //mega anime tree
C_LIGHT_GRAY, // invisible perv
C_WHITE, //holy slayer
C_BROWN, //super-size lunchkin
C_MAGENTA, //greatest yardworm
C_MAGENTA //chaos cactus
};

const char xsizes[MAX_MONSTER_B] = { 1, 1, 1, 3, 2, 1, 1, 2, 2, 3, 2, 2, 1, 2, 1, 2, 3, 1, 3, 3, 4, 2 };

const string names[MAX_MONSTER_B] = { "lunchkin", "double snake", "toam", "poof", "giant batling", "yardworm", "anime tree",
	"pie monster", "big lunchkin", "chainsaw wielding space orc", "big toam", "greater yardworm", "kinetic kid",
	"malevolent abstraction", "stalactite", "enormous toam", "mega anime tree", "invisible perv", "holy slayer",
	"super-size lunchkin", "greatest yardworm", "chaos cactus" };

// Some monsters don't act every turn:
const char act_every[MAX_MONSTER_B] = { 1, 1, 2/*toam*/, 1, 1, 1, 2/*anime tree*/, 1, 1, 1, 2/*big toam*/, 2/*greater yardworm*/,
	1, 3/*malevolent abstraction*/, 2/*stalactite*/, 3/*enormous toam*/, 3/*mega anime tree*/, 1, 1, 2/*super-size lunchkin*/,
	3/*greatest yardworm*/, 2 /*chaos cactus*/ };

}

extern Map curmap;

Monster::Monster() : prev_move_dir(D_N), turns_stationary(0) {}
Monster::~Monster() {}

void Monster::init(const short initialB)
{
	if(!initialB) // if making PC
	{
		name = "you";
		B = 1;
		E = 100;
		fig.init_app("@", 1, C_LIGHT_BLUE);
	}
	else
	{
		B = m_draw.index(initialB)+1;
		if(B > MAX_MONSTER_B)
			B = MAX_MONSTER_B;
		E = 100*B;
		name = names[B-1];
		fig.init_app(apps[B-1], xsizes[B-1], colours[B-1]);
		wait_turns = act_every[B-1];
	}
}


Figure* Monster::get_fig_ref() { return &fig; }

string Monster::get_name() const { return name; }

int Monster::getB() const { return B; }
int Monster::getE() const { return E; }
void Monster::setE(const int newE) { E = newE; }
void Monster::incrB(const int amount) { B += amount; }

bool Monster::decrE(const int amount)
{
	E -= amount;
	return E <= 0;
}


Coords Monster::get_pos() const { return fig.get_pos(); }
void Monster::set_pos(const Coords &c) { fig.set_pos(c); }


void Monster::rest()
{
	if(E < 100*B)
		++E;
}


char Monster::trymove(const e_Dir d)
{
	fig.set_pos(fig.get_pos().in(d)); // make a tentative move

	if(curmap.collision(fig)) // check to hit walls
	{
		fig.set_pos(fig.get_pos().in(!d));
		return 0; // couldn't move
	}
	// check to hit other monsters:
	for(list<Monster>::iterator i = monsters.begin(); i != monsters.end(); ++i)
	{
		if(&(*i) != this && i->get_fig_ref()->hits(fig))
		{
			fig.set_pos(fig.get_pos().in(!d));
			return 2; // hit another monster
		}
	}

	// moved; see if left the map:
	if(fig.on_map())
		return 1; // moved, is still on map

	// moved out of map! Cancel move so caller can refer to "pos before leaving"
	fig.set_pos(fig.get_pos().in(!d));
	return -1;
}


char Monster::move_towards(const Coords &c)
{
	if(--wait_turns) // not our turn yet
		return 0;
	// else:
	wait_turns = act_every[B-1];

	vector<e_Dir> dirs; // collect possible directions to move to
	char retval;
	for(char a = 0; a < 8; a++)
	{
		retval = trymove(e_Dir(a));
		if(retval == 1) // can move there
		{
			dirs.push_back(e_Dir(a));
			fig.set_pos(fig.get_pos().in(!e_Dir(a))); //cancel move
		}
	}

	// get optimal direction (which minimizes euclidian distance to c)
	char dist = 50, x;
	e_Dir d = D_N;
	for(vector<e_Dir>::const_iterator i = dirs.begin(); i != dirs.end(); ++i)
	{
		if((x = c.dist_eucl(fig.get_pos().in(*i))) < dist)
		{
			dist = x;
			d = *i;
		}
		else if(x == dist // an "equally good" path as one found previously
			&& b_draw.one_in(2)) // sometimes change
			d = *i;
	}
	if(dist < 50) // there exists an optimal direction
	{
		retval = trymove((prev_move_dir = d));
		if(retval != 1) // could not move to optimal direction
		{
			if(++turns_stationary > 4)
				return move_randomly(true);
		}
		else clear_stationary();
		return retval;
	}
	return 0;
}


char Monster::move_randomly(const bool internal)
{
	if(!internal) // a non-internal call; waiting not yet checked
	{
		if(--wait_turns) // not our turn yet
			return 0;
		// else:
		wait_turns = act_every[B-1];
	}
	char retval;
	// try moving to the same direction as previously:
	if((retval = trymove(prev_move_dir)) != 1)
	{
		prev_move_dir = e_Dir(m_draw.index(8)); // couldn't; try turning somewhere else
		retval = trymove(prev_move_dir);
	}
	if(retval == 1)
		clear_stationary();
	// not moving in random way does not increment turns_stationary!	
	return retval;
}


void Monster::cancel_prev_move() { trymove(!prev_move_dir); }


void Monster::eat(const string foodname, const char food_value)
{
	if(name == "you")
		print_msg("You eat " + art(foodname) + '.');
	if(E < 900*B/10)
	{
		E += B*10*(food_value+1);
		if(E > 900*B/10)
			E = 900*B/10;
	}
}



char Monster::get_dist_to(const Coords &c)
{
	char x,y;
	char d = 100, tmp;
	for(x = fig.get_pos().x; x < fig.get_pos().x + fig.get_xsize(); x++)
	{
		for(y = fig.get_pos().y; y < fig.get_pos().y + fig.get_ysize(); y++)
		{
			if((tmp = char(c.dist_eucl(Coords(x,y)))) < d)
				d = tmp;
		}
	}
	return d;
}

