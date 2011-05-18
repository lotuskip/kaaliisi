#include "io.h"
#include "game.h"
#include "stuff.h"

#include <iostream>

using namespace std;

string killer = "";
int killcount = 0;

int main()
{
	if(!IO::init())
		return 1;

	IO::colour(IO::C_LIGHT_BLUE);
	IO::clr_print("Kaaliisi the roguelike\n\n\
You are the blue '@'. Use numpad keys 1-9 to move (5 to rest), enter\n\
'Q' to quit, and 'i' to view your inventory.\n\
Collect '=' symbols to have something in your inventory.\n\
Collect '0' symbols (food) to increase your energy (shown in the\n\
top-left corner). Kill monsters to become better. Collecting items\n\
also increases your powers. The aim is to first become very powerful\n\
and then leave the dungeon.\n\
The monsters will try to kill you, and you die when your energy\n\
drops to 0.\n\
Good luck. Now press any key.");

	Kaaliisi::init();
	char result = Kaaliisi::run();

	IO::deinit();

	cout << endl;
	switch(result)
	{
	case 0: cout << "Quitter!"; break;
	case -1: cout << "You, the unnamed, epicene adventurer were killed by "
			<< IO::art(killer) << '.' << endl << "You managed to kill ";
			if(!killcount)
				cout << "no one";
			else if(killcount == 1)
				cout << "a monster, though,";
			else
				cout << killcount << " monsters, though,";
			cout << " and gathered " << Item::get_inventory_size() << " different items.";
			break;
	case 1: cout << "Congratulations; after killing " << killcount
			<< " monsters and having gathered "
			<< Item::get_inventory_size() << " different items, YOU WIN! Isn't that swell.";
			break;
	case 2: cout << "You escape the dungeon never to return.";
			break;
	}

	cout << endl << endl;
	return 0;
}
