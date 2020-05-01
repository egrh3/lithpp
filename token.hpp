#ifndef H_TOKEN
#define H_TOKEN

#include <variant>

struct _node;
typedef std::variant<_node*, std::string, int> token;

typedef struct _node {
private:
    _node* prev;
    token head;
    token tail;

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

    void set_value(token v) {
	if (std::get<_node*>(head) == nullptr) {
	    head = v;
	}

	else if (std::get<_node*>(tail) == nullptr) {
	    tail = v;
	}
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
	std::cout << "token -> push :: poke!\n";
	this->prev = parent;
    }

    _node* pop() {
	std::cout << "token -> pop :: thwip!\n";
	std::cout << "token -> pop :: prev@" << prev << "\n";
	return(this->prev);
    }

    bool full() {
	return (
	    (std::get<_node*>(head) != nullptr) &&
	    (std::get<_node*>(tail) != nullptr)
	);
    }

    void setop(void*) {
    }

    void purge() {
	try {
	    _node* h = std::get<_node*>(head);
	    h->purge();
	    delete(h);
	}
	catch (const std::bad_variant_access&) {
	}

	try {
	    _node* t = std::get<_node*>(tail);
	    t->purge();
	    delete(t);
	}
	catch (const std::bad_variant_access&) {
	}
    }
} node;

node* expr = nullptr;

#endif
