#include <locale>	// ctype functions: isalnum, &c..

#include "token.hpp"

// for prototyping, assume C. should probably pull in from ENV
std::locale filter("C");

// flag used to determine if a string input is a pure integer.
// I should eventually be aware of floats and different bases.
bool pureint;

size_t cull(std::string tupin, size_t idx, std::string* op) {
    size_t len = 0;

    // force set and check the first character.
    unsigned char ch = tupin[idx+len];
    pureint = isdigit(ch);

    std::cout << "LITHP :: culling --> ";
    while(isalnum(ch)) {

	// only check isdigit() if we're already though to be a number
	if (pureint && !isdigit(ch))
	    pureint = false;

	std::cout << ch;
	ch = tupin[idx + (++len)];
    }
    std::cout << '\n';

    *op = tupin.substr(idx, len);

    return(len);
}

// returns a count of the tokens found.
int parse(std::string tupin) {
    size_t idx = 0;
    size_t tks = 0;
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

		// a bound element () counts as a token
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

		// strings, numbers, and other bindings are tokens.
		if (std::ispunct(ch, filter)) {
		    std::cout << "LITHP :: parse --> operator\n";

		    tks++;
		}

		else if (std::isalnum(ch, filter)) {
		    std::cout << "LITHP :: parse --> token\n";
		    std::string op;

		    // need to offset by one because the idx incrementor is
		    //   part of the character selection above.
		    idx -= 1;
		    idx += cull(tupin, idx, &op);
		    std::cout << "LITHP :: parse --> culled token: " << op << '\n';
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
