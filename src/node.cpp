#include "node.h"

Node::Node() {
    unique = true;
    visited = false;
    
    across_gateway_bits = 0b0;
    down_gateway_bits = 0b0;
}


Node::~Node() {
    // destroy all child nodes
    for (Node* node_ptr : node_vec) {
        if (node_ptr != nullptr) {
            delete node_ptr;
        }
    }
}


Node* Node::add_child(char letter) {
    int index = letter - 'a';
    
    if (index + 1 > static_cast<int>(node_vec.size())) {
        node_vec.resize(index + 1);
    }
    
    Node* child = node_vec.at(index);
    // if a child already exists, return it
    // and mark the node as not unique
    if (child != nullptr) {
        child->unique = false;
    } else {
        // otherwise, create a new one
        child = new Node();
        
        node_vec[index] = child;
        
        // randomly insert a char into char_list
        // RNG ensures different crossword boards will be generated each time
        int randindex = randutil::randint(0, static_cast<int>(char_list.length()));
        char_list.insert(randindex, std::string(1, letter));
    }
    
    return child;
}

void Node::set_gateway_bits(Direction direction, unsigned short gateway_bits) {
    if (direction == ACROSS) {
        across_gateway_bits |= gateway_bits;
    } else {
        down_gateway_bits |= gateway_bits;
    }
}

Node* Node::get_next_node(char letter) {
    int index = letter - 'a';
    if (index > static_cast<int>(node_vec.size() - 1)) {        
        return nullptr;
    }
    Node* next_node = node_vec.at(index);
    return next_node;
}

bool Node::is_path_allowed(int word_index, Direction word_direction) {
    unsigned short gateway_bit = (1 << word_index);
    if (word_direction == ACROSS) {
        return (across_gateway_bits & gateway_bit) != 0;
    } else {
        return (down_gateway_bits & gateway_bit) != 0;
    }
}
