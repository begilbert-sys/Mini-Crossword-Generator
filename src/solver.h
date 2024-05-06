#ifndef SOLVER_H
#define SOLVER_H

#include <string>
#include <fstream>
#include <set>

#include "fixedtrie.h"
#include "matrix.h"
#include "board.h"
#include "utils/strutil.h"

#define WORD_MIN 3
#define WORD_MAX 8

#define FIXEDTRIES (WORD_MAX - WORD_MIN) + 1


class Solver {
public:
    Solver(std::string wordbank_filename);
    ~Solver();
    bool solve(Board& board);
private:
    struct NodeBookmark {
        Node* across;
        Node* down;
    };

    FixedTrie* fixedtrie_array[FIXEDTRIES];
    std::vector<std::string> words;

    void build_tries(std::string wordbank_filename);

    Coordinate next_coordinate(Coordinate coord, Board& board);

    bool recur(Coordinate coord, Board& board, Matrix<NodeBookmark>& DP);
};

#endif
