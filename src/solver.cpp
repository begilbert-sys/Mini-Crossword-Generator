#include "solver.h"
void Solver::build_tries(std::string filename) {
    std::ifstream file(filename);

    std::string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            int len = (int)line.length();
            if (len <= WORD_MAX && len >= WORD_MIN && strutil::islower(line)) {
                int trie_index = len - WORD_MIN;

                FixedTrie* trie = fixedtrie_array[trie_index];

                trie->add(line);
                words.push_back(line);
            }
        }
        file.close();
    } else {
        throw std::invalid_argument("file: \"" + filename + "\" could not be found");
    }
}


Solver::Solver(std::string filename) {
    for (int i = 0; i < FIXEDTRIES; i++) {
        fixedtrie_array[i] = new FixedTrie(i + WORD_MIN);
    }
    build_tries(filename);
}

Solver::~Solver() {
    for (int i = 0; i < FIXEDTRIES; i++) {
        delete fixedtrie_array[i];
    }
}

Coordinate Solver::next_coordinate(Coordinate coord, Board& board) {
    if (coord.column == board.columns - 1) {
        return {coord.row + 1, 0};
    }
    return {coord.row, coord.column + 1};
}

bool Solver::recur(Coordinate coord, Board& board, Matrix<NodeBookmark>& DP) {
    
    char original_value = board.get(coord);
    const NodeBookmark& bookmark = DP.get(coord);
    
    bool final_square = (coord.row == board.rows - 1) && (coord.column == board.columns - 1);
    if (original_value == BLACKOUT) {
        if (final_square) {
            return true;
        }
        return recur(next_coordinate(coord, board), board, DP);
    }
      
    // lambda for gathering info on the previous nodes
    auto get_prev_node = [&](Board::Direction direction) -> Node* {
        int word_length = (direction == Board::Direction::ACROSS) ? bookmark.across_word_length : bookmark.down_word_length;
        // if the current square is NOT the beginning of an across word, assign left_node to the previous letter's node
        if (word_length == 0) {
            if (direction == Board::Direction::ACROSS) {
                return DP.get({coord.row, coord.column - 1}).across;
            } else {
                return DP.get({coord.row - 1, coord.column}).down;
            }
            
        } else {
            // if the current square IS the beginning of an across word. . .
            // if the length is greater than 2, assign it to the head of a trie
            if (word_length > 2) {
                return fixedtrie_array[word_length - WORD_MIN]->head;
            } else {
                return nullptr;
            }
        }
    };

    
    Node* left_node = get_prev_node(Board::Direction::ACROSS);
    Node* top_node = get_prev_node(Board::Direction::DOWN);
    
    
    // lambda sets the square to a specific letter and solves from there
    auto attempt_solve = [&](char letter) -> bool {
        int index = letter - 'a';
        board.set(coord, letter);
        if (final_square) {
            return true;
        }
        
        Node* next_left_node = nullptr;
        if (left_node != nullptr) {
            next_left_node = left_node->node_array[index];
            if (next_left_node != nullptr && next_left_node->unique) {
                if (next_left_node->visited) {
                    return false;
                }
                next_left_node->visited = true;
            }
        }
        Node* next_top_node = nullptr;
        if (top_node != nullptr) {
            next_top_node = top_node->node_array[index];
            if (next_top_node != nullptr && next_top_node->unique) {
                if (next_top_node->visited) {
                    if (next_left_node->visited) {
                        next_left_node->visited = false;
                    }
                    return false;
                }
                next_top_node->visited = true;
            }
        }
        DP.set(coord, {next_left_node, next_top_node, bookmark.across_word_length, bookmark.down_word_length});
        bool result = recur(next_coordinate(coord, board), board, DP);
        
        // remove blocked nodes
        if (next_left_node->visited) {
            next_left_node->visited = false;
        }
        if (next_top_node->visited) {
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
        return false;
    }
    else if (top_node == nullptr) {
        for (const char& letter : left_node->char_list) {
            if (attempt_solve(letter)) {
                return true;
            }
        }
        return false;
    }
    
    if (original_value == BLANK) {
        Node* outer_node;
        Node* inner_node;
        if (left_node->char_list.length() > top_node->char_list.length()) {
            outer_node = top_node;
            inner_node = left_node;
        } else {
            outer_node = left_node;
            inner_node = top_node;
        }

        for (const char& letter : outer_node->char_list) {
            int index = letter - 'a';
            if (inner_node->node_array[index] != nullptr) {
                if (attempt_solve(letter)) {
                    return true;
                };
            }
        }
    }
    else {
        int index = original_value - 'a';
        Node* top_node_result = top_node->node_array[index];
        Node* left_node_result = left_node->node_array[index];
        if (top_node_result != nullptr && left_node_result != nullptr) {
            if (attempt_solve(original_value)) {
                return true;
            };
        }
    }
    board.set(coord, original_value);
    return false;
}

bool Solver::solve(Board& board) {
    std::unordered_set<std::string> added_words;
    for (const std::unordered_map<Coordinate, std::string>& wordlist : {board.across_words, board.down_words}) {
        for (auto it = wordlist.begin(); it != wordlist.end(); it++) {
            std::string word = it->second;
            int length = (int)word.length();
            if (!strutil::contains(word, BLANK) && !fixedtrie_array[length - WORD_MIN]->contains(word)) {
                added_words.insert(word);
                fixedtrie_array[length - WORD_MIN]->add(word);
            }
        }
    }
    Matrix<NodeBookmark> DP(board.rows, board.columns);
    for (int row = 0; row < board.rows; row++) {
        for (int col = 0; col < board.columns; col++) {
            Coordinate coord({row, col});
            NodeBookmark bookmark;
            bookmark.across = nullptr;
            bookmark.down = nullptr;
            auto across_word_it = board.across_words.find(coord);
            bookmark.across_word_length = (across_word_it == board.across_words.end()) ? 0 : (int)across_word_it->second.length();
            auto down_word_it = board.down_words.find(coord);
            bookmark.down_word_length = (down_word_it == board.down_words.end()) ? 0 : (int)down_word_it->second.length();
            DP.set(coord, bookmark);
        }
    }
    
    bool result = recur({0, 0}, board, DP);
    for (const std::string& word : added_words) {
        int length = (int)word.length();
        fixedtrie_array[length - WORD_MIN]->remove(word);
    }
    return result;
}
