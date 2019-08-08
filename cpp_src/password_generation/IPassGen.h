#pragma once

#include <string>
#include <vector>

static const std::string SUB_GEN = "sub_generators";
static const std::string PASS_TYPE = "type";
static const std::string OR = "or";
static const std::string CARTESIAN = "cartesian";
static const std::string DICTIONARY = "dictionary";
static const std::string BIG_DICTIONARY = "big_dictionary";
static const std::string CHARACTER = "character";
static const std::string CHARACTERS = "characters";
static const std::string PATH = "path";


class IPassGen {
public:
    virtual size_t GetLength() const = 0;
    virtual size_t GetMaxPassLength() const = 0;
    virtual size_t operator()(char *password, size_t index) const = 0;
};
