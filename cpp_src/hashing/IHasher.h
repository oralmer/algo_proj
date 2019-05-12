#pragma once

#include <string>

#define PASSWORD "password"
#define P "p"
#define TESTS "tests"
#define CUTOFF "cutoff"

class IHasher {
public:
    virtual bool operator()(std::string password) const = 0;
};


