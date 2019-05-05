#include <utility>
#include "Hasher.h"

std::string Hasher::operator()(std::string password) {
    switch (m_hash_type) {
        case id:
            return IdHash(std::move(password));
        default:
            throw "invalid enum in password generator";
    }
}

std::string Hasher::IdHash(std::string password) {
    return password;
}

Hasher::Hasher(enum HashType hash_type) : m_hash_type(hash_type) {}
