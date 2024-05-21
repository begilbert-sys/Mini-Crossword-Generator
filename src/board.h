#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cctype>

#include "globals.h"
#include "matrix.h"
#include "utils/strutil.h"

class Board {
public:

    int rows;
    int columns;
    
    Matrix<int>* across_indices;
    Matrix<int>* down_indices;


    Board(std::string board_string);
    ~Board();
    
    char get(Coordinates coords);
    void set(Coordinates coords, char letter);

    std::vector<std::string> get_wordlist(Direction direction);
    
    int get_word_index(Coordinates coords, Direction direction);
    
    int get_word_length(Coordinates coords, Direction direction);
    
    std::string to_string();
    
    // these are for debugging purposes
    void display();
    void display_words();
    void display_indices(Direction direction);

private:
    std::vector<std::string> across_words;
    std::vector<std::string> down_words;
    
    /* this has to be a pointer because idk the size at compile time */
    Matrix<char>* board;
    
    /* keeps track of which across word and down word each
     square is a part of
     */

    
    void build_board(std::string board_string);
    
    /* assign each square its word index  */
    void build_words(Direction direction);
};

#endif
