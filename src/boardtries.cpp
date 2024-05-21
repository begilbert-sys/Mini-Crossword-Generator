#include "boardtrie.h"

BoardTrie::BoardTrie(Board& board, std::string wordbank_filename) {
    board = board;
    head = new Node();
    
    // loop through both wordlists and add completed words to the trie
    for (const std::vector<std::string>& vec : {board.get_wordlist(ACROSS), board.get_wordlist(DOWN)}) {
        for (const std::string& word : vec) {
            if (!strutil::contains(word, BLANK)) {
                add_word(board, word);
            }
        }
    }
    
    load_wordbank(board, wordbank_filename);
}

BoardTrie::~BoardTrie() {
    delete head;
}

void BoardTrie::load_wordbank(Board& board, std::string wordbank_filename) {
    std::ifstream file(wordbank_filename);

    std::string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            add_word(board, line);
        }
        file.close();
    } else {
        throw std::invalid_argument("file: \"" + wordbank_filename + "\" could not be found");
    }
}

void BoardTrie::add_word(Board& board, std::string word) {
    unsigned short across_gateway_bits = generate_gateway_bits(board.get_wordlist(ACROSS), word);
    unsigned short down_gateway_bits = generate_gateway_bits(board.get_wordlist(DOWN), word);
    // if the word doesn't fit into any of the board's slots, do nothing
    if ((across_gateway_bits | down_gateway_bits) == 0) {
        return;
    }
    Node* current_node = head;
    for (const char& c : word) {
        current_node = current_node->add_child(c);
        current_node->set_gateway_bits(ACROSS, across_gateway_bits);
        current_node->set_gateway_bits(DOWN, down_gateway_bits);
    }
    
}


unsigned short BoardTrie::generate_gateway_bits(std::vector<std::string> board_words, const std::string& word) {
    unsigned short gateway_bits = 0b0;
    for (int i = 0; i < board_words.size(); i++) {
        const std::string board_word = board_words[i];
        // reject if different lengths
        if (board_word.length() != word.length()) {
            continue;
        }
        // reject if any of the letters don't match
        for (int i = 0; i < word.length(); i++) {
            if (board_word[i] != BLACKOUT && board_word[i] != word[i]) {
                continue;
            }
        }
        gateway_bits |= (1 << i);
    }
    return gateway_bits;
}


