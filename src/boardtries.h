#ifndef boardtrie_h
#define boardtrie_h

#include <fstream>

#include "board.h"
#include "globals.h"
#include "node.h"



class BoardTries {
public:
    BoardTries(Board& board, std::string wordbank_filename);
    ~BoardTries();
    Node* get_head(int word_length);
    
private:
    Node* fixedtries[FIXEDTRIES];
    void load_wordbank(Board& board, std::string wordbank_filename);
    void add_word(Board& board, std::string word);
    unsigned short generate_gateway_bits(std::vector<std::string> board_words, const std::string& word);
};


#endif
