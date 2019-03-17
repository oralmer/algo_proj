#include "HelperFuncs.h"

std::ostream &operator<<(std::ostream &s, std::vector<char> t) {
    for (const char i : t) {
        if (i == '\0') {
            break;
        }
        s << i;
    }
    return s;
}