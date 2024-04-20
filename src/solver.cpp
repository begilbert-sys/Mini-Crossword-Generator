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

Node** node_unhook(Node* node) {
    if (node->node_array != nullptr && node->char_list.length() == 1) {
        Node** node_array = node->node_array;
        node->node_array = nullptr;
        return node_array;
    } else {
        return nullptr;
    }
}

void node_rehook(Node* node, Node** node_array) {
    if (node_array != nullptr) {
        node->node_array = node_array;
    }
}


bool Solver::recur(Coordinate coord, Board& board, Matrix<NodeBookmark>& DP) {
    char original_value = board.get(coord);
    
    bool final_square = (coord.row == board.rows - 1) && (coord.column == board.columns - 1);
    if (original_value == BLACKOUT) {
        if (final_square) {
            return true;
        }
        return recur(next_coordinate(coord, board), board, DP);
    }

    bool across_word = board.is_across_word(coord);
    bool down_word = board.is_down_word(coord);

    Node* left_node;
    Node* top_node;
    if (across_word) {
        left_node = fixedtrie_array[board.across_words.at(coord).length() - WORD_MIN]->head;
        
    } else {
        left_node = DP.get({coord.row, coord.column - 1}).across;
    }
    if (down_word) {
        top_node = fixedtrie_array[board.down_words.at(coord).length() - WORD_MIN]->head;
    } else {
        top_node = DP.get({coord.row - 1, coord.column}).down;
    }

    if (left_node == nullptr || top_node == nullptr || left_node->node_array == nullptr || top_node->node_array == nullptr) {
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
            if (coord.row == 0 && coord.column == 0) {
                std::cout << "Letter: " << letter << std::endl;
            }
            if (inner_node->node_array[index] != nullptr) {
                board.set(coord, letter);
                if (final_square) {
                    return true;
                }
                DP.set(coord, {left_node->node_array[index], top_node->node_array[index]});
                
                Node** left_node_array = node_unhook(left_node);
                Node** top_node_array = node_unhook(left_node);
                bool result = recur(next_coordinate(coord, board), board, DP);
                node_rehook(left_node, left_node_array);
                node_rehook(top_node, top_node_array);
                
                if (result) {
                    return true;
                }
            }
        }
    }
    else {
        int index = original_value - 'a';
        Node* top_node_result = top_node->node_array[index];
        Node* left_node_result = left_node->node_array[index];
        if (top_node_result != nullptr && left_node_result != nullptr) {
            if (final_square) {
                return true;
            }
            DP.set(coord, {left_node_result, top_node_result});
            Node** left_node_array = node_unhook(left_node);
            Node** top_node_array = node_unhook(left_node);
            bool result = recur(next_coordinate(coord, board), board, DP);
            node_rehook(left_node, left_node_array);
            node_rehook(top_node, top_node_array);
            
            if (result) {
                return true;
            }
        }
    }
    board.set(coord, original_value);
    return false;
}

bool Solver::solve(Board& board) {
    std::set<std::string> added_words;
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
    bool result = recur({0, 0}, board, DP);
    for (const std::string& word : added_words) {
        int length = (int)word.length();
        fixedtrie_array[length - WORD_MIN]->remove(word);
    }
    if (result) {
        return result;
    }
    return false;
}
