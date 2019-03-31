#pragma once

#include <string>

enum HashType {
    id
};

class Hasher {
    HashType m_hash_type;
public:
    explicit Hasher(enum HashType hash_type);

    std::string operator()(std::string password);

    static std::string IdHash(std::string password);
};

