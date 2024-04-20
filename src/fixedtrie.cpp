#include "fixedtrie.h"

Node* FixedTrie::construct_node() {
    Node* node = new Node;
    allocated_nodes++;
    node->node_array = nullptr;
    node->char_list = std::string();
    return node;
}

FixedTrie::FixedTrie(int triesize) {
    size = triesize;
    allocated_nodes = 0;
    allocated_arrays = 0;
    head = construct_node();
}

void FixedTrie::destruct_trie(Node* node) {
    if (node->node_array == nullptr) {
        delete node;
        allocated_nodes--;
        return;
    }
    for (const char& c : node->char_list) {
        int index = c - 'a';
        destruct_trie(node->node_array[index]);
    }
    delete[] node->node_array;
    allocated_arrays--;
    delete node;
    allocated_nodes--;
}

FixedTrie::~FixedTrie() {
    /*
    std::cout << "Total allocated nodes:" << allocated_nodes << std::endl;
    std::cout << "Total allocated maps:" << allocated_maps << std::endl;
    */
    destruct_trie(head);
    assert(allocated_nodes == 0);
    assert(allocated_arrays == 0);
}

void FixedTrie::verify_word(std::string word) {
    if (word.length() != size) {
        throw std::invalid_argument("word: \"" + word +  "\" must be length " + std::to_string(size));
    }
    if (!strutil::islower(word)) {
        throw std::invalid_argument("word: \"" + word +  "\" must only contain lowercase characters");
    }
}

Node* FixedTrie::add_child(Node* node, char letter) {
    Node* new_node = construct_node();
    if (node->node_array == nullptr) {
        node->node_array = new Node*[26];
        for (int i = 0; i < 26; i++) {
            node->node_array[i] = nullptr;
        }
        allocated_arrays++;
    }
    int index = letter - 'a';
    node->node_array[index] = new_node;
    int randindex = randutil::randint(0, (int)node->char_list.length());
    node->char_list.insert(randindex, std::string(1, letter));
    return new_node;
}

void FixedTrie::add(std::string word) {
    verify_word(word);
    Node* current_node = head;
    for (char& letter : word) {
        int index = letter - 'a';
        if (current_node->node_array == nullptr || current_node->node_array[index] == nullptr) {
            current_node = add_child(current_node, letter);
        } else {
            current_node = current_node->node_array[index];
        }
    }
}

bool FixedTrie::contains(std::string word) {
    Node* current_node = head;
    for (char& letter : word) {
        int index = letter - 'a';
        if (current_node->node_array != nullptr && current_node->node_array[index] != nullptr) {
            current_node = current_node->node_array[index];
        } else {
            return false;
        }
    }
    return true;
}

void FixedTrie::remove(std::string word) {
    verify_word(word);
    Node* nodes[size];
    Node* current_node = head;
    for (int i = 0; i < size; i++) {
        char letter = word[i];
        int index = letter - 'a';
        if (current_node->node_array != nullptr && current_node->node_array[index] != nullptr) {
            nodes[i] = current_node;
            current_node = current_node->node_array[letter];
        } else {
            throw std::invalid_argument("word: \"" + word +  "\" does not exist in the FixedTrie");
        }
    }
    
    assert(current_node->node_array == nullptr);
    delete current_node;
    allocated_nodes--;
    for (int i = size - 1; i >= 0; i--) {
        current_node = nodes[i];
        char letter = word[i];
        int index = letter - 'a';
        current_node->node_array[index] = nullptr;
        current_node->char_list.erase(letter);
        if (current_node->char_list.empty()) {
            delete[] current_node->node_array;
            allocated_arrays--;
            if (current_node != head) {
                delete current_node;
                allocated_nodes--;
            } else {
                current_node->node_array = nullptr;
            }
        } else {
            break;
        }
    }
}
