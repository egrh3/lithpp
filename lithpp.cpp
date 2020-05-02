#include <iostream>
#include <string>
#include <vector>

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

    std::vector<node*> chain;

    do {
	tupin = readline("\n? ");
	add_history(tupin.c_str());

	tks = parse(tupin, &chain);
	// parse() returns (-idx) on error. for one liners,
	// this will let us identify the rogue token.
	// <g> won't fix actual parser errors, though.
	if (tks < 0) {
	    std::cerr << "\n";
	    std::cerr << "input: " << tupin << "\n";
	    std::cerr << "error: ";
	    std::cerr.width(-tks);
	    std::cerr.fill(' ');
	    std::cerr << "^" << "\n";

	} else {
	    std::cout << "counted " << tks << " after parsing\n";

	    evaluate(&chain);
	    prn_chain(&chain);
	}

	chain.clear();

    } while(tupin != "(quit)");

    return 0;
}
