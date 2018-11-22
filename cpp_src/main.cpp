#include <iostream>
#include "PasswordGenerator.h"
#include "Hasher.h"

int main(int argc, char *argv[]) {
    if (argc < 6 || strcmp(argv[1], "-h") == 0) {
        std::cerr << "Usage: " << argv[0] << " [START] [END] [HASH TYPE] [PASSWORD TYPE] [HASH]" << std::endl;
    }
    //TODO: add support for combining password, vector of (pass_type, len) pairs
    int start = atoi(argv[1]); //TODO: input checking and proper conversion
    int end = atoi(argv[2]);
    auto hash_type = static_cast<HashType>(atoi(argv[3]));
    auto password_type = static_cast<PassType>(atoi(argv[4]));
    std::string password = argv[6];

    PasswordGenerator password_generator = PasswordGenerator(password_type);
    Hasher hasher = Hasher(hash_type);

    for (int i = start; i < end && i < password_generator.Count(); i++) {
        if (hasher(password_generator(i)) == password){
            std::cout << "success! password: " << password_generator(i) << std::endl;
            return 0;
        }
    }
    std::cout << "failed, password not in range." << std::endl;
    return 0;
}