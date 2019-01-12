#include <iostream>
#include <nlohmann/json.hpp>
#include "password_generation/IPassGen.h"
#include "Hasher.h"
#include "password_generation/PassGenFactory.h"

int main(int argc, char *argv[]) {
    if (argc < 6 || strcmp(argv[1], "-h") == 0) {
        std::cerr << "Usage: " << argv[0] << " [START] [END] [HASH TYPE] [PASSWORD TYPE] [HASH]" << std::endl;
        return 1;
    }
    auto start = (size_t) atoi(argv[1]); //TODO: input checking and proper conversion
    auto end = (size_t) atoi(argv[2]);
    auto hash_type = static_cast<HashType>(atoi(argv[3]));
    nlohmann::json password_params = nlohmann::json::parse(argv[4]);
    std::string password = argv[5];
    std::cerr << start << " " << end << " pass:" << password << std::endl;

    std::unique_ptr<IPassGen> password_generator = PassGenFactory::BuildPassGen(password_params);
    auto hasher = Hasher(hash_type);
    for (size_t i = start; i < end && i < password_generator->GetLength(); i++) {
        if (hasher((*password_generator)(i)) == password) {
            std::cout << R"({"result":true, "password":")" << (*password_generator)(i) << "\"}";
            return 0;
        }
    }
    std::cerr << start << " " << end << " done" << std::endl;
    std::cout << R"({"result":false, "password":null})";
    return 0;
}