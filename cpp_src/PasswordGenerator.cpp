//
// Created by or on 11/18/2018.
//

#include "PasswordGenerator.h"
#include <iostream>

PasswordGenerator::PasswordGenerator(enum PassType pass_type, int length) :
        pass_type_(pass_type),
        length_(length) {}

std::string PasswordGenerator::Numbers(int index, int length) {
    std::string password;
    for (int i = 0; i < length; i++) {
        password += static_cast<char>('0' + index % 10);
        index /= 10;
    }
    return password;
}

std::string PasswordGenerator::Characters(int index, int length) {
    std::string password;
    for (int i = 0; i < length; i++) {
        password += static_cast<char>('a' + index % 26);
        index /= 26;
    }
    return password;
}

std::string PasswordGenerator::operator()(int index) {
    switch (pass_type_) {
        case nums:
            return Numbers(index, length_);
        case chars:
            return Characters(index, length_);
        default:
            throw std::runtime_error("invalid enum in password generator");
    }
}

int PasswordGenerator::Count() {
    switch (pass_type_) {
        case nums:
            return (int) std::pow(10, length_);
        case chars:
            return (int) std::pow(26, length_);
        default:
            throw std::runtime_error("invalid enum in password generator");
    }
}
