#pragma once

#include <string>

static const std::string TESTS = "tests";
static const std::string CUTOFF = "cutoff";
static const std::string PASSWORD = "password";

class IHasher {
public:
    virtual bool operator()(std::string password) = 0;
};


