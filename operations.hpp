#ifndef H_OPERS
#define H_OPERS

#include "token.hpp"

// doubling up the operators here supports multi-char operators.
// if this weren't const, it could be modified during runtime.
// like, for defining new operators!
//    void addop(newop) { strcat(validops, newop) }
const std::string validops = "()*/++--&&";

//bool knownop(std::string tupin, std::size_t idx, std::string* op) {
bool knownop(const char op) {
    std::size_t foundop = validops.find(op);
    if (foundop != std::string::npos) {
        return(true);
    } else {
	return(false);
    }
}

void noop(token h, token t) {
}

token add(token h, token t) {
    int l = std::get<int>(h);
    int r = std::get<int>(t);
    return (l + r);
}

token sub(token h, token t) {
    int l = std::get<int>(h);
    int r = std::get<int>(t);
    return (l - r);
}

void evaluate(std::vector<node*>* chain) {
    for (auto e: *chain) {
	e->eval();
    }
}

#endif
