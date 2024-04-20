#include <iostream>
#include <string>
#include <chrono>

#include "solver.h"
#include "matrix.h"
#include "fixedtrie.h"
#include "board.h"
#include "randutil.h"


using namespace std;
// g++ -g -Wall -Wextra main.cpp fixedtrie.cpp solver.cpp strutil.cpp board.cpp; ./a.exe

const std::string DIRECTORY = "/Users/bengilbert/Documents/Mini Crossword Generator/data/";

void run() {
    srand((unsigned) time(NULL));
    auto start = chrono::high_resolution_clock::now();
    Solver solver(DIRECTORY + "words7.txt");
    Board board(DIRECTORY + "board.txt");
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    //cout << "Overhead: " << duration.count() << "ms" << endl;
    start = chrono::high_resolution_clock::now();
    solver.solve(board);
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(end - start);
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
