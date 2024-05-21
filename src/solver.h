
#ifndef solver_h
#define solver_h

#include <string>

#include "board.h"
#include "boardtries.h"
#include "globals.h"
#include "matrix.h"
#include "node.h"

namespace Solver{
std::string solve(std::string board_string, std::string wordbank_filename);
}

#endif
