#pragma once

#include <string>

#define PASSWORD "password"
#define P "p"
#define TESTS "tests"
#define CUTOFF "cutoff"
#define A "a"
#define B "b"

class IHasher {
public:
    virtual bool operator()(std::string password) const = 0;
};


