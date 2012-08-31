#ifndef MONSTER_H
#define MONSTER_H

#include <string>
#include "figure.h"

const short MAX_MONSTER_B = 25;

class Monster
{
public:
	Monster();
	~Monster();

	void init(const short initialB); // pass initialB=0 to init as PC, otherwise iB is max B for the generated monster (anything below that is possible!)

	Figure* get_fig_ref();
	int getB() const;
	int getE() const;
	void setE(const int newE);
	bool decrE(const int amount); // returns true if died
	void incrB(const int amount);

	Coords get_pos() const;
	void set_pos(const Coords &c);
	char get_dist_to(const Coords &c);

	std::string get_name() const;

	void rest();
	void eat(const std::string &foodname, const char food_value);
	char trymove(const e_Dir d); // returns 0 if couldn't move, 1 if moved, and -1 if left the map
	char move_towards(const Coords &c);
	char move_randomly(const bool internal = false);
	void cancel_prev_move();
	void clear_stationary() { turns_stationary = 0; }

private:
	std::string name;
	Figure fig; // figure describes position, dimensions, and appearance
	int B;
	int E;

	e_Dir prev_move_dir; // used when moving randomly to make it more coherent-looking(?!)
	char turns_stationary;
	char wait_turns;
};

#endif
