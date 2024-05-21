#ifndef node_h
#define node_h

#include <string>
#include <vector>
#include <cassert>

#include "globals.h"
#include "utils/randutil.h"


/* defines a node to be used in the trie */

class Node {
public:
    /* A node is marked 'unique' when its children only point to one word. In other words, none of its children branch off
     */
    bool unique;
    /*
    Unique nodes can be marked as 'visited' so that the solver doesn't use the same word twice in one board.
     */
    bool visited;
    
    /* a list of every child letter */
    std::string char_list;
    
    Node();
    
    ~Node();
    
    /* Add a letter to the node and return the newly-created node. If a child node already exists for that letter, return it.
     */
    Node* add_child(char letter);
    
    
    /*
     Allow a specific word index / direction to pass through this node.
     
     For example: if this node pointed to the word "lock" and word 3 across is "_o_k", then it would be allowed, whereas a word like "li__" would not
     */
    void set_gateway_bits(Direction direction, unsigned short gateway_bits);
    
    
    
    /* gets the next Node or nullptr if it's not accessible */
    
    Node* get_next_node(char letter, int word_index, Direction word_direction);
    
    bool is_path_allowed(int word_index, Direction word_direction);
    
    
    
    
private:
    
    /* array of child nodes */
    std::vector<Node*> node_vec;
    
    
    /*
     When a board is already pre-filled with a few letters, these bits give the solver the ability to "look ahead" at whether or not a path will succeed or fail.
     Each bit here represents the nth word on a board.
     Imagine the 4th across word on a board is "___nd". When solving the 4th word, we only want the solver to go down paths that end in "nd".
     So when we add the word "grand" to our trie. for all 5 of the added nodes, the 4th across bit will be set to 1:
     
            across_gateway_bits |= 0b0000000000001000
     
     now when the solver is solving word 4, it will look at the 4th bit and know it's okay to go down that node's path.
     
     */
    
    unsigned short across_gateway_bits;
    
    unsigned short down_gateway_bits;
};


#endif
