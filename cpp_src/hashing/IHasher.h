#pragma once

#include <string>

static const std::string TESTS = "tests";
static const std::string CUTOFF = "cutoff";
static const std::string PASSWORD = "password";

class IHasher {
public:
    //Return true <=> password matches the hash.
    virtual bool operator()(std::string password) const = 0;
};


