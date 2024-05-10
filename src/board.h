#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cctype>

#include "matrix.h"
#include "utils/strutil.h"

#define BLANK '_'
#define BLACKOUT '.'

class Board {
    public:
        enum Direction {
            ACROSS,
            DOWN
        };
    
        int rows;
        int columns;
        std::unordered_map<Coordinate, std::string> across_words;
        std::unordered_map<Coordinate, std::string> down_words;


        Board(std::string board_string);
        ~Board();

        char get(Coordinate coord);
        void set(Coordinate coord, char letter);

        bool is_across_word(Coordinate coord);
        bool is_down_word(Coordinate coord);
        
        void display();
        void display_words();

    private:
        Matrix<char>* boardptr;
        void build_words(Direction direction);

};

#endif
