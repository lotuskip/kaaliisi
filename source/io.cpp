#include "io.h"

#include <ncurses.h>
#include <iostream>
#include <cstdlib>

namespace
{
using namespace std;

bool may_clear_messages = false;
string msg_buffer = "";

const string vovels = "aeiouAEIOU";
}


bool IO::init()
{
	if(!initscr())
	{
		cout << endl << "Failed to init ncurses screen!" << endl << endl;
		return false;
	}

	int scr_x, scr_y;
	getmaxyx(stdscr, scr_y, scr_x);
	if(scr_x < screen_x || scr_y < screen_y)
	{
		endwin();
		cout << endl << "Kaaliisi requires a terminal window of size 25x80. Please adjust your terminal and run the program again."
			<< endl << endl;
		return false;
	}

	start_color();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();
	notimeout(stdscr, TRUE);
	nonl();
	wresize(stdscr,screen_y,screen_x);

	char ct[MAX_COLOR] = { COLOR_BLACK, COLOR_BLUE, COLOR_GREEN, COLOR_CYAN, COLOR_RED, COLOR_MAGENTA,
		COLOR_YELLOW, COLOR_WHITE, COLOR_BLACK, COLOR_BLUE, COLOR_GREEN, COLOR_CYAN, COLOR_RED,
		COLOR_MAGENTA, COLOR_YELLOW, COLOR_WHITE };

	for (char i = 0; i < MAX_COLOR; i++)
		init_pair(i, ct[i], COLOR_BLACK);
	return true;
}


void IO::deinit()
{
	nocbreak();
	echo();
	endwin();
}


void IO::clr()
{
	cursor(Coords(0,map_y));
	clrtobot();
	if(!may_clear_messages)
	{
		cursor(Coords(0,map_y));
		colour(C_WHITE);
		addstr(msg_buffer.c_str());
	}
	refresh();
	msg_buffer.clear();
}


void IO::cursor(const Coords &c)
{
	move(c.y, c.x);
	refresh();
}


void IO::colour(const e_Colour c)
{
	if(char(c) >= 8) attrset((A_BOLD | (COLOR_PAIR(c))));
	else attrset(COLOR_PAIR(c));
}


void IO::print(const char c) { addch(c); }
void IO::print(const string &s) { addstr(s.c_str()); }


void IO::print_msg(const string &s)
{
	Coords oldcurs = getxy();
	cursor(Coords(0,map_y));

	if(!msg_buffer.empty())
		msg_buffer += ' ';
	msg_buffer += s;
	/* Some wishlist items (TODO)
	 * (1) clip s nicely,
	 * (2) if too long to fit in three lines, ask for more after printing two,
	 * (3) if not long enough to fill 3 lines, empty the rest with clrtoeol();
	 */
	colour(C_WHITE);
	addstr(msg_buffer.c_str());

	cursor(oldcurs);
	may_clear_messages = false;
}


char IO::getkey()
{
	may_clear_messages = true;
	return getch();
}


Coords IO::getxy()
{
	refresh();
	Coords tmp;
	getyx(stdscr, tmp.y, tmp.x);
	return tmp;
}


string IO::art(const string &target)
{
	if(vovels.find(target[0],0) == string::npos)
		return "a " + target;
	return "an " + target;
}


void IO::clr_print(const string &s)
{
	may_clear_messages = true;
	clr();
	cursor(Coords(0,0));
	addstr(s.c_str());
	getkey();
}

