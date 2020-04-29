enum types {
    VT_STRING = 0,
    VT_INT,
    VT_EOL
};

enum opers {
    OP_LIST = 0,
    OP_MULTI,
    OP_DIVIS,
    OP_ADD,
    OP_SUB,
    OP_EOL
};

typedef struct _node {
private:
    struct _node* prev;
    struct _node* head;
    struct _node* tail;

    bool opened;
    bool closed;

    // an enumeration of value types
    int type;

    union value {
	std::string string;
	char	    character;
	int	    integer;
    };

public:
    _node() {
	std::cout << "node->() :: creation of new node\n";
	prev = NULL;
	head = NULL;
	tail = NULL;

	opened = true;
	closed = false;
    }

    ~_node() {
	std::cout << "node->(~) :: deletion of node\n";
	if (head != NULL)
	    head->prev = prev;
	if (tail != NULL)
	    tail->prev = prev;
    }

    struct _node* pop() {
	_node* parent = prev;
	delete(this);

	return(parent);
    }

    struct _node* back() {
	std::cout << "node->back() :: getting prev node\n";
	return(prev);
    }

    void push(_node* way) {
	std::cout << "node->push() :: adding new token to chain\n";
	way->prev = this;
    }

    bool is_closed() {
	return(this->closed);
    }

    void close() {
	std::cout << "node->close() :: closing token\n";
	this->opened = false;
	this->closed = true;
    }

    // I'm hoping to replace this (o h t) token with the result of
    // its operation. not sure how to properly do that. especially
    // with unknown / variable types.
    void eval() {
	//prev->result(this) = this->oper(head, tail);
    }

    // i think this needs to return the address of "head" or "tail"
    // i think this needs an operator= override ..
    _node* result(_node* caller) {
	if (head == caller) {
	    return(head);
	}

	else if (tail == caller) {
	    return(tail);
	}
    }

} node;

