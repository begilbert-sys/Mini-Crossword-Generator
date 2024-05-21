#include "boardtries.h"

BoardTries::BoardTries(Board& board, std::string wordbank_filename) {
    
    // set all fixed tries to nullptr by default
    for (int i = 0; i < FIXEDTRIES; i++) {
        fixedtries[i] = nullptr;
    }
    
    // loop through both wordlists
    // initialize fixed tries based on the lengths of the words
    // and add completed words to the trie
    
    for (const std::vector<std::string>& vec : {board.get_wordlist(ACROSS), board.get_wordlist(DOWN)}) {
        for (const std::string& word : vec) {
            // initialize fixed tries based on the lengths of the words
            int index = static_cast<int>(word.length()) - WORD_MIN;
            if (fixedtries[index] == nullptr) {
                fixedtries[index] = new Node();
                fixedtries[index]->unique = false;
            }
            
            // if the word has already been completed, add it to the trie
            if (!strutil::contains(word, BLANK)) {
                add_word(board, word);
            }
        }
    }
    
    load_wordbank(board, wordbank_filename);
}

BoardTries::~BoardTries() {
    for (int i = 0; i < FIXEDTRIES; i++) {
        if (fixedtries[i] != nullptr) {
            delete fixedtries[i];
        }
    }
}

void BoardTries::load_wordbank(Board& board, std::string wordbank_filename) {
    std::ifstream file(wordbank_filename);

    std::string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            if (!strutil::islower(line)) {
                throw std::logic_error("word is not lowercase: " + line);
            }
            add_word(board, line);
        }
        file.close();
    } else {
        throw std::invalid_argument("file: \"" + wordbank_filename + "\" could not be found");
    }
}

void BoardTries::add_word(Board& board, std::string word) {
    int word_length = static_cast<int>(word.length());
    int trie_index = word_length - WORD_MIN;
    // if none of the board's words are the same length as this word, ignore it
    Node* current_node = fixedtries[trie_index];
    if (current_node == nullptr) {
        return;
    }
    
    unsigned short across_gateway_bits = generate_gateway_bits(board.get_wordlist(ACROSS), word);
    unsigned short down_gateway_bits = generate_gateway_bits(board.get_wordlist(DOWN), word);
    // if the word doesn't fit into any of the board's slots, do nothing
    if ((across_gateway_bits | down_gateway_bits) == 0) {
        return;
    }
    
    
    for (const char& c : word) {
        current_node->set_gateway_bits(ACROSS, across_gateway_bits);
        current_node->set_gateway_bits(DOWN, down_gateway_bits);
        current_node = current_node->add_child(c);
    }
    
}

Node* BoardTries::get_head(int word_length) {
    int trie_index = word_length - WORD_MIN;
    return fixedtries[trie_index];
}

bool BoardTries::contains(std::string word) {
    int word_length = static_cast<int>(word.length());
    Node* current_node = get_head(word_length);
    for (const char& c : word) {
        current_node = current_node->get_next_node(c, 5, DOWN);
        if (current_node == nullptr) {
            return false;
        }
    }
    return true;
}

unsigned short BoardTries::generate_gateway_bits(std::vector<std::string> board_words, const std::string& word) {
    unsigned short gateway_bits = 0b0;
    for (size_t i = 0; i < board_words.size(); i++) {
        const std::string board_word = board_words[i];
        // reject if any of the letters don't match
        bool valid = true;
        for (size_t i = 0; i < word.length(); i++) {
            if (board_word[i] != BLANK && board_word[i] != word[i]) {
                valid = false;
                break;
            }
        }
        if (valid) {
            gateway_bits |= (1 << i);
        }
    }
    return gateway_bits;
}


