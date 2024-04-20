#ifndef FIXEDTRIE_H
#define FIXEDTRIE_H

#include <iostream>
#include <string> 
#include <unordered_map>
#include <cassert>

#include "strutil.h"
#include "randutil.h"


/*
The reason `children` is a pointer to a map is to save memory. Many nodes have no children,
so rather than creating a bunch of empty maps, nullptr can be used.
*/
struct Node {
    Node** node_array;
    std::string char_list;
};

class FixedTrie {
public:
    int size;
    Node* head;

    FixedTrie(int size);
    ~FixedTrie();
    void add(std::string word);
    void remove(std::string word);
    bool contains(std::string word);

private:
    int allocated_nodes;
    int allocated_arrays;

    void destruct_trie(Node* node);
    void verify_word(std::string word);
    Node* construct_node();
    Node* add_child(Node* node, char letter);
};

#endif
