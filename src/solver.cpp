#include "solver.h"

struct CachedNodes {
    Node* across;
    Node* down;
    
};


/* Fetch the next coordinate on the board.
 I'm not good enough at C++ to know if inline actually makes a difference here. Probably not.
*/
inline Coordinates next_coordinate(Coordinates coords, Board& board) {
    if (coords.column == board.columns - 1) {
        return {coords.row + 1, 0};
    }
    return {coords.row, coords.column + 1};
}

bool recur(Coordinates coords, Board& board, BoardTries& boardtries, Matrix<CachedNodes>& node_cache) {
    char original_value = board.get(coords);
    bool final_square = (coords.row == board.rows - 1) && (coords.column == board.columns - 1);
    // skip the square if it's blacked out
    if (original_value == BLACKOUT) {
        if (final_square) {
            return true;
        }
        return recur(next_coordinate(coords, board), board, boardtries, node_cache);
    }
    
    // important vars
    int across_index  = board.get_word_index(coords, ACROSS);
    int across_length = board.get_word_length(coords, ACROSS);
    int down_index    = board.get_word_index(coords, DOWN);
    int down_length   = board.get_word_length(coords, DOWN);
    
    
    // lambda for retrieving the left/top cached nodes
    auto get_prev_node = [&](Direction direction) -> Node* {
        int word_length = (direction == ACROSS) ? (across_length) : (down_length);
        if (word_length == -1) {
            // this means the word length is less than 3 and should be ignored
            return nullptr;
        }
        if ((direction == ACROSS && coords.column == 0) || (direction == DOWN && coords.row == 0)) {
            return boardtries.get_head(word_length);
        }
        
        Node* prev_node = (direction == ACROSS) ? (node_cache.get({coords.row, coords.column - 1}).across) : (node_cache.get({coords.row - 1, coords.column}).down);
        // in this case, the previous node was a BLACKOUT square
        if (prev_node == nullptr) {
            return boardtries.get_head(word_length);
        }
        return prev_node;
    };
    
    Node* left_node = get_prev_node(ACROSS);
    Node* top_node = get_prev_node(DOWN);
    
    if ((across_index != -1 && !left_node->is_path_allowed(across_index, ACROSS)) ||
        (down_index != -1 && !top_node->is_path_allowed(down_index, DOWN))) {
        return false;
    }
    
    // this lambda sets the square to a specific letter and solves from there
    auto attempt_solve = [&](char letter) -> bool {
        
        board.set(coords, letter);
        if (final_square) {
            return true;
        }
        
        Node* next_left_node = nullptr;
        if (left_node != nullptr) {
            next_left_node = left_node->get_next_node(letter);
            if (next_left_node->unique) {
                if (next_left_node->visited) {
                    return false;
                }
                next_left_node->visited = true;
            }
        }
        Node* next_top_node = nullptr;
        if (top_node != nullptr) {
            next_top_node = top_node->get_next_node(letter);
            if (next_top_node->unique) {
                if (next_top_node->visited) {
                    if (left_node != nullptr && next_left_node->visited) {
                        next_left_node->visited = false;
                    }
                    return false;
                }
                next_top_node->visited = true;
            }
        }
        
        node_cache.set(coords, {next_left_node, next_top_node});
        bool result = recur(next_coordinate(coords, board), board, boardtries, node_cache);
        // remove blocked nodes
        if (left_node != nullptr && next_left_node->visited) {
            next_left_node->visited = false;
        }
        if (top_node != nullptr && next_top_node->visited) {
            next_top_node->visited = false;
        }
        return result;
    };
    
    if (top_node == nullptr && left_node == nullptr) {
        return false;
    }
    else if (left_node == nullptr) {
        for (const char& letter : top_node->char_list) {
            if (attempt_solve(letter)) {
                return true;
            }
        }
        board.set(coords, original_value);
        return false;
    }
    else if (top_node == nullptr) {
        for (const char& letter : left_node->char_list) {
            if (attempt_solve(letter)) {
                return true;
            }
        }
        board.set(coords, original_value);
        return false;
    }
    
    if (original_value == BLANK) {
        if (left_node->char_list.length() > top_node->char_list.length()) {
            for (const char& letter : top_node->char_list) {
                if (left_node->get_next_node(letter) != nullptr) {
                    if (attempt_solve(letter)) {
                        return true;
                    };
                }
            }
            
        } else {
            for (const char& letter : left_node->char_list) {
                if (top_node->get_next_node(letter) != nullptr) {
                    if (attempt_solve(letter)) {
                        return true;
                    };
                }
            }
        }
    }
    else {
        if (left_node->get_next_node(original_value) != nullptr &&
            top_node->get_next_node(original_value) != nullptr) {
            if (attempt_solve(original_value)) {
                return true;
            };
        }
    }
    
    board.set(coords, original_value);
    return false;
}



std::string Solver::solve(std::string board_string, std::string wordbank_filename) {
    Board board(board_string);
    BoardTries boardtries(board, wordbank_filename);
    
    Matrix<CachedNodes> node_cache(board.rows, board.columns);
    node_cache.fill({nullptr, nullptr});
    recur({0, 0}, board, boardtries, node_cache);
    return board.to_string();
    
}
