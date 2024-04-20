#ifndef STRUTILS_H
#define STRUTILS_H

#include <string>
#include <cctype>

namespace strutil {
    std::string tolower(std::string str);
    std::string toupper(std::string str);

    bool islower(std::string str);
    bool isupper(std::string str);

    bool contains(std::string, char c);
}
#endif