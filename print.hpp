#ifndef H_PRINT
#define H_PRINT

#include "token.hpp"

extern node* expr;

void prn_expr() {
    std::cout << "expr: " << expr << "\n";
}

#endif
