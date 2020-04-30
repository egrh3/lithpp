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
    node* n = new(node);

    e->oper = (*add);
    e->tail = n;
    e->head = 245;

    n->oper = (*add);
    n->head = 44;
    n->tail = 11;

    stub r = e->eval();
    std::visit([](auto x) { std::cout << x << "\n";}, r);
    return 0;
}
