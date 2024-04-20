#include "strutil.h"

std::string strutil::tolower(std::string str) {
    std::string newstr = "";
    for (char& c : str) {
        newstr += std::tolower(c);
    }
    return newstr;
}

std::string strutil::toupper(std::string str) {
    std::string newstr = "";
    for (char& c : str) {
        newstr += std::toupper(c);
    }
    return newstr;
}

bool strutil::islower(std::string str) {
    for (char& c : str) {
        if (!std::islower(c)) {
            return false;
        }
    }
    return true;
}

bool strutil::isupper(std::string str) {
    for (char& c : str) {
        if (!std::isupper(c)) {
            return false;
        }
    }
    return true;
}

bool strutil::contains(std::string str, char chr) {
    for (char& c : str) {
        if (c == chr) {
            return true;
        }
    }
    return false;
}