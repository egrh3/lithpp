#ifndef H_TOKEN
#define H_TOKEN

#include <variant>


struct _node;
typedef std::variant<_node*, std::string, int> token;

typedef token (*op)(token, token);

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

enum fill_lvl {
    OPEN = 0,
    HALF,
    FULL,
    EOL
};

typedef struct _node {
private:
    _node* prev;
    token head;
    token tail;

    unsigned int fill;
    unsigned int head_t;
    unsigned int tail_t;

    // operational function
    token (*oper) (token, token);

    bool opened;
    bool closed;

public:
    _node() {
	prev = nullptr;
	head = nullptr;
	tail = nullptr;

	opened = true;
	closed = false;

	oper = nullptr;
	fill = OPEN;
    }

    friend std::ostream& operator<< (std::ostream& outs, _node& RHS) {
	outs << "expr(" << &RHS      << "); ";
	outs << "prev(" << &(RHS.prev) << ")";

	return(outs);
    }

    token eval() {
	std::cout << "LITHP :: eval --> eval(head)\n";
	try {
	    head = std::get<_node*>(head)->eval();
	    head = this->oper(head, tail);
	}
	catch (const std::bad_variant_access&) {
	}

	std::cout << "LITHP :: eval --> eval(tail)\n";
	try {
	    tail = std::get<_node*>(tail)->eval();
	    tail = this->oper(head, tail);
	}
	catch (const std::bad_variant_access&) {
	}

	// original call; doesn't assign anything to head
	//tail = this->oper(head, tail);
	return tail;
    };

    void push_value(token v) {
	switch(fill) {
	    case OPEN:
		this->head = v;
		break;

	    case HALF:
		this->tail = v;
		break;

	    case FULL:
	    default:
		std::cerr << "token :: push_two --> OOPS! full node\n";
		return;

	}

	fill++;
    }

    bool is_closed() {
	return(closed);
    }

    void close() {
	if (opened && !closed) {
	    this->opened = false;
	    this->closed = true;
	}
    }

    void push(_node* parent) {
	std::cout << "token -> push(" << this << ") ";
	std::cout << ":: parent(" << parent << "\n";
	this->prev = parent;
    }

    _node* pop() {
	std::cout << "token -> pop :: thwip!\n";
	std::cout << "token -> pop :: prev@" << prev << "\n";
	return(this->prev);
    }

    bool full() {
	return (fill == FULL);
    }

    void setop(op fn) {
	std::cout << "token -> steop :: sting<- operation set\n";
	this->oper = fn;
    }

} node;


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
    std::cout << "op(add) :: " << l+r << "\n";
    return (l + r);
}

token sub(token h, token t) {
    int l = std::get<int>(h);
    int r = std::get<int>(t);
    std::cout << "op(sub) :: " << l-r << "\n";
    return (l - r);
}

void evaluate(std::vector<node*>* chain) {
    std::cout << "LITHP :: evaluate -> calling into chain\n";
    for (auto e: *chain) {
	e->eval();
    }
}

op opmap(const char ch) {
    switch(ch) {
	case '+':
	    std::cout << "opmap -> function 'add' found for " << ch << "\n";
	    return (*add);

	default:
	    return (nullptr);
    }
}

#endif
