#include <iostream>
#include "PasswordGenerator.h"
#include "Hasher.h"

int main(int argc, char *argv[]) {
    if (argc < 6 || strcmp(argv[1], "-h") == 0) {
        std::cerr << "Usage: " << argv[0] << " [START] [END] [HASH TYPE] [PASSWORD TYPE] [HASH]" << std::endl;
        return 1;
    }
    //TODO: add support for combining password, vector of (pass_type, len) pairs
    auto start = (size_t)atoi(argv[1]); //TODO: input checking and proper conversion
    auto end = (size_t)atoi(argv[2]);
    auto hash_type = static_cast<HashType>(atoi(argv[3]));
    auto password_type = static_cast<PassType>(atoi(argv[4]));
    std::string password = argv[5];
    std::cerr << start << " " << end << " pass:" << password << std::endl;
    PasswordGenerator password_generator = PasswordGenerator(password_type);
    Hasher hasher = Hasher(hash_type);

    for (size_t i = start; i < end && i < password_generator.IterSize(); i++) {
        if (hasher(password_generator(i)) == password) {
            std::cout << R"({"result":true, "password":")" << password_generator(i) << "\"}";
            return 0;
        }
    }
    std::cerr << start << " " << end << " done" << std::endl;
    std::cout << R"({"result":false, "password":null})";
    return 0;
}