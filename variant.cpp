// good ideas: https://www.bfilipek.com/2018/06/variant.html
//
#include <variant>
#include <iostream>

struct _node;
typedef std::variant<_node*, std::string, int> stub;

typedef struct _node {
    _node* prev;
    stub head;
    stub tail;

    stub (*oper) (stub, stub);

    stub eval() {
	try {
	    head = std::get<_node*>(head)->eval();
	}
	catch (const std::bad_variant_access&) {
	}

	try {
	    tail = std::get<_node*>(tail)->eval();
	}
	catch (const std::bad_variant_access&) {
	}

	tail = this->oper(head, tail);
	return tail;
    };
} node;

stub add(stub h, stub t) {
    int l = std::get<int>(h);
    int r = std::get<int>(t);
    return (l + r);
}

int main(int argc, char *argv[])
{
    node* e = new(node);

    //(+ (+ 22 11) (+ 33 44))
    e->oper = (*add);

    node* f = new(node);
    f->oper = (*add);
    f->head = 22;
    f->tail = 11;
    e->head = f;

    node* g = new(node);
    g->oper = (*add);
    g->head = 33;
    g->tail = 44;
    e->tail = g;

    std::visit([](auto x) { std::cout << x << "\n";}, e->eval());
    return 0;
}
