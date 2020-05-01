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

	tks = parse(tupin);
	if (tks < 0) {
	    std::cerr << "parsing error(" << tks << "). try again\n";
	} else {
	    std::cout << "counted " << tks << " after parsing\n";
	}

	prn_expr();

	// so broken
	//expr.purge();

	delete(expr);
	expr = nullptr;

    } while(tupin != "(quit)");

    return 0;
}
