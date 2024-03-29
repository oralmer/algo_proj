
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "../HelperFuncs.h"
#include "../password_generation/PassGenFactory.h"
#include "../test_resources/PerformanceIn/GenerationIn.h"
#include "../test_resources/PerformanceIn/BigDictIn.h"
#include "../test_resources/HasherIn/EChasher_in.h"
#include "../test_resources/HasherIn/ModPHasher_in.h"
#include "../hashing/HasherFactory.h"

void TestGenerationSpeed(const std::string& params){
    nlohmann::json test_params = nlohmann::json::parse(params);
    std::unique_ptr<IPassGen> password_generator = PassGenFactory::BuildPassGen(test_params);
    std::vector<char> password_buffer(password_generator->GetMaxPassLength() + 1);
    clock_t start_time = clock();
    for (size_t i = 0; i < password_generator->GetLength(); i++) {
        (*password_generator)(password_buffer.data(), i);
    }
    double total_time = double(clock() - start_time) / double(CLOCKS_PER_SEC);
    std::cout << "\ntime taken: " << total_time << '\n';
}

void TestHashingSpeed(const std::string& generation_in, const std::string& hasher_in){
    nlohmann::json generation_params = nlohmann::json::parse(generation_in);
    nlohmann::json hash_params = nlohmann::json::parse(hasher_in);
    std::unique_ptr<IPassGen> password_generator = PassGenFactory::BuildPassGen(generation_params);
    std::unique_ptr<IHasher> hasher = HasherFactory::BuildHasher(hash_params);
    std::vector<char> password_buffer(password_generator->GetMaxPassLength() + 1);
    clock_t start_time = clock();

    for (size_t i = 0; i < 10000; i++) {
        (*password_generator)(password_buffer.data(), i);
        (*hasher)(password_buffer.data());
    }
    double total_time = double(clock() - start_time) / double(CLOCKS_PER_SEC);
    std::cout << "\ntime taken: " << total_time << '\n';
}

TEST(performance, mixed_generation) {
    TestGenerationSpeed(generation_pref_in); // 6468000 passwords
}
//test 1 - 5.078
//test 2 - 47.392 # after temporarily replacing dict with big_dict,
//                # but this test uses a small dictionary, requiring lots of resets
//                # see the big dict test for performance with no resets.

TEST(performance, big_dict_generation) { // 14344392 passwords
    TestGenerationSpeed(big_dict_in);
}
//test 1 - 4.313

TEST(performance, modPHasher) {
    TestHashingSpeed(generation_pref_in, ModPHasher_in);
}
//test 1 - 0.218

TEST(performance, ECHasher) {
    TestHashingSpeed(generation_pref_in, EChasher_in);
}
//test 1 - 7.297 # beginning of first EC pull request
//test 2 - 5.765 # after moving vectors to members and not re-copying the constant parts of the curr_in vector
//test 3 - 4.375 # after fixing HMAC
//test 4 - 0.516 # after adding early exiting in ThresholdHasher - The smallest fixes shine the brightest.