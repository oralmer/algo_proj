//
// Created by or on 11/18/2018.
//

#ifndef CPP_SRC_PASSWORD_GENERATOR_H
#define CPP_SRC_PASSWORD_GENERATOR_H


#include <string>
#include <tuple>

enum PassType {
    chars, nums
};

class PasswordGenerator {
    PassType pass_type_;

    static std::string Numbers(int index);

    static std::string Characters(int index);

    static std::tuple<int, int> CalcLengthCartesian(int index, int base);

public:
    explicit PasswordGenerator(enum PassType pass_type);

    std::string operator()(int index);

    int Count();

};


#endif //CPP_SRC_PASSWORD_GENERATOR_H
