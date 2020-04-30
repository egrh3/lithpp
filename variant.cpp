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
	    tail = this->oper(head, tail);
	}

	return tail;
    };
} node;

int main(int argc, char *argv[])
{
    node e;
    node n;

    e.head = "ndht";
    std::visit([](auto x) { std::cout << x << '\n';}, e.head);

    n.tail = 52345;
    std::visit([](auto x) { std::cout << x << '\n';}, n.tail);

    e.tail = &n;
    return 0;
}
