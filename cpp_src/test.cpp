#include <gtest/gtest.h>
#include <unordered_set>
#include <iostream>
#include <nlohmann/json.hpp>
#include "password_generation/IPassGen.h"
#include "password_generation/PassGenFactory.h"
#include "HelperFuncs.h"

#include "test_resources/PassGenIn/cartesian_in.h"
#include "test_resources/PassGenIn/dict_in.h"
#include "test_resources/PassGenIn/or_in.h"
#include "test_resources/PassGenIn/sanity_in.h"
#include "test_resources/PassGenOut/cartesian_out.h"
#include "test_resources/PassGenOut/dict_out.h"
#include "test_resources/PassGenOut/or_out.h"
#include "test_resources/PassGenOut/sanity_out.h"

bool RunPassGenTest(const std::string &input_string, const std::unordered_set<std::string> &expected_output) {
    nlohmann::json test_params = nlohmann::json::parse(input_string);

    std::unique_ptr<IPassGen> password_generator = PassGenFactory::BuildPassGen(test_params);
    std::vector<char> password_buffer(password_generator->GetMaxPassLength() + 1);
    std::unordered_set<std::string> real_output;
    for (size_t i = 0; i<password_generator->GetLength(); i++) {
        try {
            (*password_generator)(password_buffer.data(), i);
        }
        catch (const std::exception &e) {
            std::cout << "exception for:\n";
            std::cout << test_params << '\n';
            std::cout << "at index: " << i << '\n';
            return false;
        }
        std::string result(password_buffer.data());
        real_output.insert(result);
    }
    return real_output == expected_output;
}

TEST(PasswordGeneration, sanity) {

    ASSERT_TRUE(RunPassGenTest(sanity_in, sanity_out));
}

TEST(PasswordGeneration, dict) {
    ASSERT_TRUE(RunPassGenTest(dict_in, dict_out));
}

TEST(PasswordGeneration, cartesian) {
    ASSERT_TRUE(RunPassGenTest(cartesian_in, cartesian_out));
}

TEST(PasswordGeneration, or_) {
    ASSERT_TRUE(RunPassGenTest(or_in, or_out));
}