#include <utility>

//
// Created by or on 11/19/2018.
//

#include "Hasher.h"

std::string Hasher::operator()(std::string password) {
    switch (hash_type_) {
        case id:
            return IdHash(std::move(password));
        default:
            throw std::runtime_error("invalid enum in password generator");
    }
}

std::string Hasher::IdHash(std::string password) {
    return password;
}

Hasher::Hasher(enum HashType hash_type) : hash_type_(hash_type) {}
