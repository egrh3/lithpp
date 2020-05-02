#ifndef H_TOKEN
#define H_TOKEN

#include <variant>

struct _node;
typedef std::variant<_node*, std::string, int> token;

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
	//outs << "expr(" << &RHS      << "); ";
	//outs << "prev(" << &(RHS.prev) << ")";
	for (auto c: *RHS) {
	    std::visit(overloaded {[](auto n) { outs << n; } }, c);
	}

	return(outs);
    }

    token eval() {
	try {
	    head = std::get<_node*>(head)->eval();
	    head = this->oper(head, tail);
	}
	catch (const std::bad_variant_access&) {
	}

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

    // this is GOING to fail. if the active type is not the one
    // requested, the variant throws std::bad_variant_access. I
    // should be able to use that here and in pick().
    bool full() {
	return (fill == FULL);
    }

    void setop(void*) {
    }

} node;

#endif
