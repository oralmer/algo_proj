#pragma once

#include <string>
#include <vector>

#define SUB_GEN "sub_generators"
#define TYPE "type"
#define OR "or"
#define CARTESIAN "cartesian"
#define DICTIONARY "dictionary"
#define CHARACHTER "character"
#define CHARACHTERS "characters"
#define PATH "path"


class IPassGen {
public:
    virtual size_t GetLength() const = 0;
    virtual size_t GetMaxPassLength() const = 0;
    virtual size_t operator()(char *password, size_t index) const = 0;
};
