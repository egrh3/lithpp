#ifndef H_PRINT
#define H_PRINT

#include <vector>

#include "token.hpp"

void prn_chain(std::vector<node*>* chain) {
    std::cout << "\n\n--> parsed " << chain->size() << " expression(s):\n";
    for (auto v: *chain) {
	std::cout << "\t- " << *v << "\n";
    }
}

#endif
