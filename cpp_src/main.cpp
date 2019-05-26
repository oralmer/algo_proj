#include <iostream>
#include <nlohmann/json.hpp>
#include "password_generation/IPassGen.h"
#include "password_generation/PassGenFactory.h"
#include "hashing/IHasher.h"
#include "hashing/HasherFactory.h"
#include "HelperFuncs.h"
//TODO: benchmark - regular + check hasher and passgen separately on flattened list (as test)

int main(int argc, char *argv[]) {
    if (argc < 5 || strcmp(argv[1], "-h") == 0) {
        std::cerr << "Usage: " << argv[0] << " [START] [END] [HASH TYPE] [PASSWORD TYPE]" << std::endl;
        return 1;
    }
    auto start = (size_t) atoi(argv[1]); //TODO: input checking and proper conversion
    auto end = (size_t) atoi(argv[2]);
    nlohmann::json hash_params = nlohmann::json::parse(argv[3]);
    nlohmann::json password_params = nlohmann::json::parse(argv[4]);
    std::cerr << start << " " << end << " hash:" << hash_params << " pass:" << password_params << std::endl;
    std::unique_ptr<IPassGen> password_generator = PassGenFactory::BuildPassGen(password_params);
    std::unique_ptr<IHasher> hasher = HasherFactory::BuildHasher(hash_params);
    std::vector<char> password_buffer(password_generator->GetMaxPassLength() + 1);
    nlohmann::json results;
    results["results"] = nlohmann::json::array();
    clock_t start_time = clock();
    for (size_t i = start; i < end && i < password_generator->GetLength(); i++) {
        (*password_generator)(password_buffer.data(), i);
        if ((*hasher)(password_buffer.data())) {
            results["results"].push_back(std::string(password_buffer.data()));
        }
    }
    double total_time = double(clock() - start_time) / double(CLOCKS_PER_SEC);
    std::cerr << "time taken: " << total_time;
    std::cout << results;
    return 0;
}