#include "board.h"

Board::Board(std::string board_string) {
    build_board(board_string);
    
    across_indices = new Matrix<int>(rows, columns);
    down_indices   = new Matrix<int>(rows, columns);
    
    build_words(ACROSS);
    build_words(DOWN);
}


Board::~Board() {
    delete board;
    delete across_indices;
    delete down_indices;
}


void Board::build_board(std::string board_string) {
    std::vector<std::string> contents;
    std::string rowstr = "";
    for (const char& c : board_string) {
        switch (c) {
            case ' ':
                break;

            case BLANK:
            case BLACKOUT:
                rowstr += c;
                break;

            case '\n':
                contents.push_back(rowstr);
                rowstr = "";
                break;

            default:
                if (!(std::islower(c))) {
                    throw new std::logic_error(std::string("\"") + c + "\" is not a valid letter");
                }
                rowstr += c;
                break;
        }
    }
    if (!rowstr.empty()) {
        contents.push_back(rowstr);
    }
    rows = (int)contents.size();
    columns = (int)contents[0].size();
    board = new Matrix<char>(rows, columns);

    for (int row = 0; row < rows; row++) {
        if ((int)contents[row].size() != columns) {
            throw new std::logic_error("Row \"" + contents[row] + "\" is not a valid length");
        }
        for (int col = 0; col < columns; col++) {
            set({row, col}, contents[row][col]);
        }
    }
}

void Board::build_words(Direction direction) {
    std::string current_string = "";
    Coordinates current_coordinates = {0, 0};
    int current_index = 0;
    bool index_disabled = false;
    
    bool reset;
    
    // initalize indices board and fill it with default value of -1
    Matrix<int>* indices = (direction == ACROSS) ? (across_indices) : (down_indices);
    indices->fill(-1);
    
    int primary = (direction == ACROSS) ? (rows) : (columns);
    int secondary = (direction == ACROSS) ? (columns) : (rows);
    
    for (int p = 0; p < primary; p++) {
        reset = true;
        for (int s = 0; s < secondary; s++) {
            int row = (direction == ACROSS) ? (p) : (s);
            int col = (direction == ACROSS) ? (s) : (p);
            current_coordinates = {row, col};
            if (reset) {
                if (current_string.length() > 2) {
                    if (direction == ACROSS) {
                        across_words.push_back(current_string);
                        current_index++;
                    } else {
                        down_words.push_back(current_string);
                        current_index++;
                    }
                }
                current_string = "";
                reset = false;
                
                
                // if the next three letters aren't free, then it's not a word. disable it.
                // this code block is kind of cancer. I'm sorry.
                if (direction == ACROSS) {
                    index_disabled = (col + 3 > columns) || (get({row, col + 1}) == BLACKOUT) || (get({row, col + 2}) == BLACKOUT);
                } else {
                    index_disabled = (row + 3 > rows) || (get({row + 1, col}) == BLACKOUT) || (get({row + 2, col}) == BLACKOUT);
                }
                
            }
            char letter = board->get(current_coordinates);
            if (letter == BLACKOUT) {
                reset = true;
                continue;
            }
            
            current_string += letter;
            if (index_disabled) {
                // this occurs with words of less than length 3
                indices->set(current_coordinates, -1);
            } else {
                indices->set(current_coordinates, current_index);
            }
        }
    }
    if (current_string.length() > 2) {
        if (direction == ACROSS) {
            across_words.push_back(current_string);
        } else {
            down_words.push_back(current_string);
        }
    }
}

std::vector<std::string> Board::get_wordlist(Direction direction) {
    if (direction == ACROSS) {
        return across_words;
    } else {
        return down_words;
    }
}

int Board::get_word_index(Coordinates coords, Direction direction) {
    if (direction == ACROSS) {
        return across_indices->get(coords);
    } else {
        return down_indices->get(coords);
    }
}

int Board::get_word_length(Coordinates coords, Direction direction) {
    size_t word_length;
    if (direction == ACROSS) {
        int indices = across_indices->get(coords);
        if (indices == -1) {
            return -1;
        }
        word_length = across_words.at(indices).size();
    }
    else {
        int indices = down_indices->get(coords);
        if (indices == -1) {
            return -1;
        }
        word_length = down_words.at(indices).size();
    }
    return static_cast<int>(word_length);
}

char Board::get(Coordinates coords) {
    return board->get(coords);
}
void Board::set(Coordinates coords, char letter) {
    board->set(coords, letter);
}


std::string Board::to_string() {
    std::string board_string = "";
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            board_string += get({row, col});
        }
        board_string += '\n';
    }
    return board_string;
}


void Board::display() {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            std::cout << board->get({row, col}) << " ";
        }
        std::cout << std::endl;
    }
}

void Board::display_words() {
    std::cout << "ACROSS:" << std::endl;
    for (const std::string& word : across_words) {
        std::cout << word << std::endl;
    }
    std::cout << std::endl;
    std::cout << "DOWN:" << std::endl;
    for (const std::string& word : down_words) {
        std::cout << word << std::endl;
    }
}

void Board::display_indices(Direction direction) {
    Matrix<int>* indices = (direction == ACROSS) ? (across_indices) : (down_indices);
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            std::cout << indices->get({row, col}) << " ";
        }
        std::cout << std::endl;
    }
    
}
