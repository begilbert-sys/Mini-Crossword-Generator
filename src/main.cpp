#include <iostream>
#include <string>
#include <chrono>
#include <filesystem>
#include <fstream>

#include "solver.h"
#include "matrix.h"
#include "fixedtrie.h"
#include "board.h"
#include "utils/randutil.h"


using namespace std;

const string DIRECTORY = filesystem::current_path().string();

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
    Solver solver(DIRECTORY + "/data/words.txt");
    Board board(read_board(DIRECTORY + "/data/board.txt"));
    auto start = chrono::high_resolution_clock::now();
    solver.solve(board);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    board.display();
    cout << "Elapsed time: " << duration.count() << "ms" << endl;
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
