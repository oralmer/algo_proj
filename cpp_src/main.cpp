#include <iostream>
#include "password_generation/IPassGen.h"
#include "Hasher.h"
#include "password_generation/PassGenFactory.h"

int main(int argc, char *argv[]) {
    if (argc < 6 || strcmp(argv[1], "-h") == 0) {
        std::cerr << "Usage: " << argv[0] << " [START] [END] [HASH TYPE] [PASSWORD TYPE] [HASH]" << std::endl;
        return 1;
    }
    auto start = (size_t)atoi(argv[1]); //TODO: input checking and proper conversion
    auto end = (size_t)atoi(argv[2]);
    auto hash_type = static_cast<HashType>(atoi(argv[3]));
    std::string password_type = argv[4];
    std::string password = argv[5];
    std::cerr << start << " " << end << " pass:" << password << std::endl;
//    PasswordGenerator password_generator = PasswordGenerator(password_type);

    PassGenParams cart_sub1;
    cart_sub1.m_part_type = character;
    cart_sub1.m_additional_data = "abcdefg";
    PassGenParams cart_sub2;
    cart_sub2.m_part_type = character;
    cart_sub2.m_additional_data = "hijklmnop";
    std::vector<PassGenParams> cart_sub = {cart_sub1, cart_sub2};
    PassGenParams or_sub1;
    or_sub1.m_part_type = cartesian;
    or_sub1.m_sub_parts = cart_sub;
    PassGenParams or_sub2;
    or_sub2.m_part_type = character;
    or_sub2.m_additional_data = "123456789";
    std::vector<PassGenParams> or_sub = {or_sub1, or_sub2};
    PassGenParams params;
    params.m_part_type = or_;
    params.m_sub_parts = or_sub;

    std::unique_ptr<IPassGen> password_generator = PassGenFactory::BuildPassGen(params);
    Hasher hasher = Hasher(hash_type);

    for (size_t i = start; i < end && i < password_generator->GetLength(); i++) {
//        std::cout << (password_generator->operator()(i)) << "\n";
        if (hasher(password_generator->operator()(i)) == password) {
            std::cout << R"({"result":true, "password":")" << password_generator->operator()(i) << "\"}";
            return 0;
        }
    }
    std::cerr << start << " " << end << " done" << std::endl;
    std::cout << R"({"result":false, "password":null})";
    return 0;
}