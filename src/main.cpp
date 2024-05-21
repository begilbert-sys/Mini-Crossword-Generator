#include <iostream>
#include <string>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <thread>

#include "solver.h"
#include "board.h"
#include "boardtries.h"


using namespace std;

const string DIRECTORY = "/Users/bengilbert/Documents/Mini Crossword Generator";

string read_board(string filename) {
    cout << filename << endl;
    ifstream file(filename);
    string line;
    string result = "";

    if (file.is_open()) {
        while (getline(file, line)) {
            result += line + '\n';
        }
    } else {
        throw std::invalid_argument("file: \"" + filename + "\" could not be found");
    }
    return result;

}


void run() {
    srand((unsigned) time(NULL));
    //Board board(read_board(DIRECTORY + "/data/board.txt"));
    //auto start = chrono::high_resolution_clock::now();
    //auto end = chrono::high_resolution_clock::now();
    //auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    //BoardTries(board, DIRECTORY + "/data/words.txt");
    string board_string = read_board(DIRECTORY + "/data/board.txt");
    string solved_string = Solver::solve(board_string, DIRECTORY + "/data/words.txt");
    cout << solved_string << endl;
    //cout << "Elapsed time: " << duration.count() << "ms" << endl;
}

int main() {
    try {
        run();
        cout << "Program exited successfully" << endl;
        return EXIT_SUCCESS;
    } catch (const exception& e) {
        cerr << "AN ERROR OCCURRED:\n" << e.what() << endl;
        return EXIT_FAILURE;
    }
}
