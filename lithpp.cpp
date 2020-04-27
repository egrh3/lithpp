#include <iostream>
#include <string>

// C libraries. with some coaxing, these should be helpful.
#include <readline/readline.h>
#include <readline/history.h>

#include "parser.hpp"

int main(int argc, char *argv[])
{
    using_history();

    int tks = 0;
    std::string tupin;

    do {
	tupin = readline("\n? ");
	add_history(tupin.c_str());

	tks = parse(tupin);
	std::cout << "counted " << tks << " after parsing\n";
    } while(tupin != "(quit)");

    return 0;
}
