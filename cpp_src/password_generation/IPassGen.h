#pragma once

#include <string>
#include <vector>

#define SUB_GEN "sub_genertors"
#define TYPE "type"
#define OR "or"
#define CARTESIAN "cartesian"
#define DICTIONARY "dictionary"
#define CHARACHTER "character"
#define CHARACHTERS "characters"
#define PATH "path"

enum PassPartType {
    cartesian,
    or_,
    dict,
    character
};

struct PassGenParams {
    PassPartType m_part_type;
    std::vector<PassGenParams> m_sub_parts;
    std::string m_additional_data;
};


class IPassGen {
public:
    virtual size_t GetLength() const = 0;
    virtual size_t GetMaxPassLength() const = 0;
    virtual size_t operator()(char *password, size_t index) const = 0;
};
