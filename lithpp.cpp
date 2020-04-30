#include <iostream>
#include <string>

// C libraries. with some coaxing, these should be helpful.
#include <readline/readline.h>
#include <readline/history.h>

#include "parser.hpp"
#include "print.hpp"

int main(int argc, char *argv[])
{
    using_history();

    int tks = 0;
    std::string tupin;

    do {
	tupin = readline("\n? ");
	add_history(tupin.c_str());

	node* expr = nullptr;

	tks = parse(tupin, expr);
	if (tks < 0) {
	    std::cerr << "parsing error(" << tks << "). try again\n";
	} else {
	    std::cout << "counted " << tks << " after parsing\n";
	}

	prn_expr(expr);
	delete(expr);

    } while(tupin != "(quit)");

    return 0;
}
