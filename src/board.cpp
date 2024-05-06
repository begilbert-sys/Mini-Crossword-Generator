#include "board.h"

void Board::build_words(bool is_across) {
    if (is_across) {
        across_words.clear();
    } else {
        down_words.clear();
    }
    std::string current_string = "";
    Coordinate current_coord = {0, 0};
    bool reset;
    
    int primary = is_across ? rows : columns;
    int secondary = is_across ? columns : rows;
    
    for (int p = 0; p < primary; p++) {
        reset = true;
        for (int s = 0; s < secondary; s++) {
            int row = is_across ? p : s;
            int col = is_across ? s : p;
            if (reset) {
                if (!current_string.empty()) {
                    if (is_across) {
                        across_words[current_coord] = current_string;
                    } else {
                        down_words[current_coord] = current_string;
                    }
                }
                current_string = "";
                current_coord = {row, col};
                reset = false;
            }
            char letter = boardptr->get({row, col});
            if (letter == BLACKOUT) {
                reset = true;
                continue;
            }
            current_string += letter;
        }
    }
    if (!current_string.empty()) {
        if (!strutil::contains(current_string, BLANK)) {
            current_string = strutil::toupper(current_string);
        }
        if (is_across) {
            across_words[current_coord] = current_string;
        } else {
            down_words[current_coord] = current_string;
        }
    }
}

Board::Board(std::string board_string) {
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
                std::cout << rowstr << std::endl;
                contents.push_back(rowstr);
                rowstr = "";
                break;

            default:
                if (!islower(c)) {
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
    boardptr = new Matrix<char>(rows, columns);

    for (int row = 0; row < rows; row++) {
        if ((int)contents[row].size() != columns) {
            throw new std::logic_error("Row \"" + contents[row] + "\" is not a valid length");
        }
        for (int col = 0; col < columns; col++) {
            set({row, col}, contents[row][col]);
        }
    }
    build_words(true);
    build_words(false);
}

Board::~Board() {
    delete boardptr;
}

char Board::get(Coordinate coord) {
    return boardptr->get(coord);
}
void Board::set(Coordinate coord, char letter) {
    boardptr->set(coord, letter);
}


bool Board::is_across_word(Coordinate coord) {
    return across_words.count(coord) == 1;
}

bool Board::is_down_word(Coordinate coord) {
    return down_words.count(coord) == 1;
}


void Board::display() {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            std::cout << boardptr->get({row, col}) << " ";
        }
        std::cout << std::endl;
    }
}

void Board::display_words() {
    std::cout << "ACROSS:" << std::endl;
    for (auto it = across_words.begin(); it != across_words.end(); it++) {
        std::cout << "(" << it->first.row << "," << it->first.column << ") " << it->second << std::endl;
    }
    std::cout << std::endl;
    std::cout << "DOWN:" << std::endl;
    for (auto it = down_words.begin(); it != down_words.end(); it++) {
        std::cout << "(" << it->first.row << "," << it->first.column << ") " << it->second << std::endl;
    }
}
