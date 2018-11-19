//
// Created by or on 11/18/2018.
//

#ifndef CPP_SRC_PASSWORD_GENERATOR_H
#define CPP_SRC_PASSWORD_GENERATOR_H


#include <string>

enum PassType {
    chars, nums
};

class PasswordGenerator {
    PassType pass_type_;
    int length_;

    static std::string Numbers(int index, int length);

    static std::string Characters(int index, int length);

public:
    explicit PasswordGenerator(enum PassType pass_type, int length);

    std::string operator()(int index);

    int Count();

};


#endif //CPP_SRC_PASSWORD_GENERATOR_H
