#include <locale>	// ctype functions: isalnum, &c..

#include "token.hpp"

auto& ch_filter = std::use_facet<std::ctype<char>>(std::locale());

size_t cull(std::string tupin, size_t idx, std::string* op) {
    size_t len = 0;
    unsigned char ch = tupin[idx+len];

    std::cout << "LITHP :: culling --> ";
    while(isalnum(ch)) {
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
    unsigned char ch = '\0';	// holder value rather than empty string.

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
		// strings, numbers, and other bindings are tokens.
		if (ch_filter.is(std::ctype_base::punct, ch)) {
		    std::cout << "LITHP :: parse --> operator\n";

		    tks++;
		}

		else if (ch_filter.is(std::ctype_base::alnum, ch)) {
		    std::cout << "LITHP :: parse --> token\n";
		    std::string op;

		    // need to offset by one because the idx incrementor is
		    //   part of the character selection above.
		    idx += cull(tupin, idx-1, &op);
		    std::cout << "LITHP :: parse --> culled token: " << op << '\n';
		    tks++;
		}

		else {
		    std::cout << "LITHP :: parse --> parse error\n";
		    return(-1);
		}
	}
    }

    return(tks);
}
