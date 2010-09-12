#ifndef IO_H
#define IO_H

#include "coords.h"

#include <string>
#include <boost/lexical_cast.hpp>

namespace IO
{
	enum e_Colour { C_BLACK, C_BLUE, C_GREEN, C_CYAN, C_RED, C_MAGENTA, C_BROWN,
		C_LIGHT_GRAY, C_DARK_GRAY, C_LIGHT_BLUE, C_LIGHT_GREEN, C_LIGHT_CYAN,
		C_LIGHT_RED, C_LIGHT_MAGENTA, C_YELLOW, C_WHITE, MAX_COLOR };

	bool init();
	void deinit();

	void clr(); // clear screen; retains messages if necessary
	void cursor(const Coords &c);
	void colour(const e_Colour c);
	Coords getxy();

	void print(const char c); // prints c at cursor's current pos
	void print(const std::string &s); // prints s at cursor's current pos
	void print_msg(const std::string &s); // prints s at message buffer
	void clr_print(const std::string &s); // clears the screen, prints the requested and asks for input

	char getkey();

	std::string art(const std::string &target);

	// to convert 1234 => "1234" for instance
	template<class T, class S> T lex_cast(S arg) { return boost::lexical_cast<T>(int(arg)); }

	const char screen_x = 80;
	const char screen_y = 25;
	const char map_y = 22;
}

#endif
