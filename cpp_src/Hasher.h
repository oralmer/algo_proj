//
// Created by or on 11/19/2018.
//

#ifndef CPP_SRC_HASHER_H
#define CPP_SRC_HASHER_H

#include <string>

enum HashType {
    id
};

class Hasher {
    HashType hash_type_;
public:
    explicit Hasher(enum HashType hash_type);

    std::string operator()(std::string password);

    static std::string IdHash(std::string password);
};


#endif //CPP_SRC_HASHER_H
