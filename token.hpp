
typedef struct _node {
private:
    struct _node* prev;
    struct _node* head;
    struct _node* tail;

    bool opened;
    bool closed;

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
	_node* parent = NULL;
	if (prev != NULL)
	    parent = prev;
	delete(this);

	return(parent);
    }

    struct _node* back() {
	std::cout << "node->pop() :: getting prev node\n";
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
} node;

