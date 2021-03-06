#include <locale>	// ctype functions: isalnum, &c..
#include <vector>
#include <cstdlib>

#include "token.hpp"

// for prototyping, assume C. should probably pull in from ENV
std::locale filter("C");

// flag used to determine if a string input is a pure integer.
// I should eventually be aware of floats and different bases.
bool pureint;

// I want to change the return of cull() to the discovered token type.
void cull(std::string tupin, std::size_t idx, std::string* tok) {
    std::size_t len = 0;

    // force set and check the first character.
    unsigned char ch = tupin[idx+len];
    pureint = isdigit(ch);

    std::cout << "LITHP :: culling --> ";
    while(isalnum(ch)) {

	// only check isdigit() if we're already thought to be a number
	if (pureint && !isdigit(ch))
	    pureint = false;

	std::cout << ch;
	ch = tupin[idx + (++len)];
    }
    std::cout << '\n';

    *tok = tupin.substr(idx, len);

    //return(len);
}

// how do I handle input like "()()" ? a vector<node>?
// returns a count of the tokens found.
int parse(std::string tupin, std::vector<node*>* chain) {
    std::size_t idx = 0;
    std::size_t tks = 0;
    char ch = '\0';	// temp value rather than empty string.

    // for each call, generate a new expression
    node* newt = nullptr;
    node* expr = nullptr;

    std::cout << "expr: " << expr << " / newt: " << newt << "\n";

    while((ch = tupin[idx++]) != '\0') {
	std::cout << "LITHP :: parse --> peek: " << ch << '\n';

	switch(ch) {
	    case '(':
		std::cout << "LITHP :: parse --> open\n";

		newt = new(node);
		if (expr) {
		    if (!expr->is_closed()) {
			newt->push(expr);
		    }
		}

		expr = newt;
		chain->push_back(expr);

		std::cout << "LITHP :: parse(open) --> expr @" << expr << '\n';
		break;

	    case ')':
		std::cout << "LITHP :: parse --> close\n";
		if ((expr == nullptr) || expr->is_closed()) {
		    std::cerr << "parsing error, unexpected closure\n";
		    return(-idx);
		}

		expr->close();

		if (newt = expr->pop()) {
		    std::cout << "LITHP :: parse(close) --> up one\n";
		    expr = newt;
		}

		// any bound element () counts as a token
		std::cout << "LITHP :: parse(close) --> expr @" << expr << '\n';
		tks++;
		break;

	    case ' ':
		std::cout << "LITHP :: parse --> space\n";
		break;

	    default:
		if (expr == nullptr) {
		    // this input dosen't error: "(+ 2 3) ndh()"
		    std::cerr << "parsing error, identifier before open expression\n";
		    return (-idx);
		}

		else if (expr->full()) {
		    std::cerr << "unexpected operand. expected '(' or ')'\n";
		    return (-idx);
		}

		// we have a valid expression object. parse a token and
		// populate the object. prefer head, then tail.
		if (std::ispunct(ch, filter)) {
		    std::cout << "LITHP :: parse --> operator? ";
		    if (knownop(ch)) {
			std::cout << "valid\n";
			// only allows single character operations
			expr->setop(opmap(ch));

			tks++;
		    } else {
			std::cout << "UNKOP\n";
			return (-idx);
		    }
		}

		// strings, numbers, and other bindings are tokens.
		else if (std::isalnum(ch, filter)) {
		    std::cout << "LITHP :: parse --> token\n";
		    std::string tok;
		    token tmp;

		    // need to offset by one because the idx incrementor is
		    //   part of the character selection above.
		    idx -= 1;

		    // keep going until we encounter a non-alnum character
		    cull(tupin, idx, &tok);
		    idx += tok.length();
		    std::cout << "LITHP :: parse --> culled token: " << tok << '\n';

		    if (tupin[idx + 1] == '(') {
			std::cout << "LITHPP :: parse --> identified function head\n";
		    }

		    else if (pureint) {
			tmp = std::atoi(tok.c_str());
			std::cout << "LITHPP :: parse --> identified pure integer\n";
		    }

		    else {
			tmp = tok;
		    }

		    tks++;
		    expr->push_value( tmp );

		}

		else {
		    std::cerr << "LITHP :: parse --> parse error\n";
		    return(-idx);
		}
	}
    }

    if (!expr->full()) {
	std::cerr << "LITHP :: parse --> incomplete expression. no currying.\n";
	return(-idx);
    }

    if (!expr->is_closed()) {
	std::cerr << "LITHP :: parse --> input finished without closing expr\n";
	return(-idx);
    }

    else {
        return(tks);
    }
}
