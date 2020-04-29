#include <locale>	// ctype functions: isalnum, &c..

#include "token.hpp"

// for prototyping, assume C. should probably pull in from ENV
std::locale filter("C");

// flag used to determine if a string input is a pure integer.
// I should eventually be aware of floats and different bases.
bool pureint;

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

// returns a count of the tokens found.
int parse(std::string tupin) {
    std::size_t idx = 0;
    std::size_t tks = 0;
    char ch = '\0';	// temp value rather than empty string.

    // for each call, generate a new expression
    node* expr = NULL;
    node* newt = NULL;

    while((ch = tupin[idx++]) != '\0') {
	std::cout << "LITHP :: parse --> peek: " << ch << '\n';

	switch(ch) {
	    case '(':
		std::cout << "LITHP :: parse --> open\n";
		// in most every case, we simply create a new node
		// we use 'newt' to create the new token we are parsing.
		newt = new(node);
		if (expr != NULL)
		    expr->push(newt);
		else
		    expr = newt;

		std::cout << "LITHP :: parse(open) --> newt @" << newt << '\n';
		break;

	    case ')':
		std::cout << "LITHP :: parse --> close\n";
		if ((newt == NULL) || newt->is_closed()) {
		    std::cout << "parsing error, unexpected closure\n";
		    return(-2);
		}
		newt->close();
		newt = newt->back();

		// any bound element () counts as a token
		std::cout << "LITHP :: parse(close) --> newt @" << newt << '\n';
		tks++;
		break;

	    case ' ':
		std::cout << "LITHP :: parse --> space\n";
		break;

	    default:
		if (expr == NULL) {
		    // this input dosen't error: "(+ 2 3) ndh()"
		    std::cout << "parsing error, identifier encountered before open expression\n";
		    return (-3);
		}

		else if (expr->full()) {
		    std::cout << "unexpected operand. expected '(' or ')'\n";
		    return (-5);
		}

		// we have a valid expression object. parse a token and
		// populate the object. prefer head, then tail.
		if (std::ispunct(ch, filter)) {
		    std::cout << "LITHP :: parse --> operator? ";
		    if (knownop(ch)) {
			std::cout << "valid\n";

			expr->setop(ch);

			tks++;
		    } else {
			std::cout << "UNKOP\n";
			return (-4);
		    }
		}

		// strings, numbers, and other bindings are tokens.
		else if (std::isalnum(ch, filter)) {
		    std::cout << "LITHP :: parse --> token\n";
		    std::string tok;

		    // need to offset by one because the idx incrementor is
		    //   part of the character selection above.
		    idx -= 1;

		    // keep going until we encounter a non-alnum character
		    cull(tupin, idx, &tok);
		    idx += tok.length();
		    std::cout << "LITHP :: parse --> culled token: " << tok << '\n';

		    tks++;

		    if (pureint) {
			std::cout << "LITHPP :: parse --> identified pure integer\n";
		    }

		    if (tupin[idx] == '(') {
			std::cout << "LITHPP :: parse --> identified function head\n";
		    }
		}

		else {
		    std::cout << "LITHP :: parse --> parse error\n";
		    return(-1);
		}
	}
    }

    return(tks);
}
