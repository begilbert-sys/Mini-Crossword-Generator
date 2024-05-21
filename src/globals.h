#ifndef globals_h
#define globals_h

// blank character
inline const char BLANK = '_';

// blakced-out character
inline const char BLACKOUT = '.';

// min word size
inline const int WORD_MIN = 3;

// max word size (don't increase this number)
inline const int WORD_MAX = 8;

// the number of tries to create
inline const int FIXEDTRIES = (WORD_MAX - WORD_MIN) + 1;

enum Direction {
    ACROSS,
    DOWN
};

#endif
