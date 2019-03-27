#include <gtest/gtest.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "password_generation/IPassGen.h"
#include "Hasher.h"
#include "password_generation/PassGenFactory.h"
#include "HelperFuncs.h"

static const std::string tests_input_dir = R"(C:\Users\or\PycharmProjects\algo_proj\algo_proj\cpp_src\test_resources\PassGenIn\)";
static const std::string tests_output_dir = R"(C:\Users\or\PycharmProjects\algo_proj\algo_proj\cpp_src\test_resources\PassGenOut\)";

bool RunPassGenTest(const std::string &filename) {
    std::ifstream input_file(tests_input_dir + filename);
    std::ifstream output_file(tests_output_dir + filename);
    nlohmann::json test_params;
    input_file >> test_params;

    std::unique_ptr<IPassGen> password_generator = PassGenFactory::BuildPassGen(test_params);
    size_t i = 0;
    std::vector<char> password_buffer(password_generator->GetMaxPassLength() + 1);
    for (std::string line; getline(output_file, line);) {
//        line.erase(std::remove(line.begin(), line.end(), [](char c) { return c == '\n' || c == '\r'; }), line.end());
        line.erase( std::remove(line.begin(), line.end(), '\r'), line.end() );
        line.erase( std::remove(line.begin(), line.end(), '\n'), line.end() );
        if (i > password_generator->GetLength()) {
            std::cout << "index out of bounds for:\n";
            std::cout << test_params << '\n';
            std::cout << "at index: " << i << '\n';
            return false;
        }
        try {
            (*password_generator)(password_buffer.data(), i);
        }
        catch (const std::exception &e) {
            std::cout << "exception for:\n";
            std::cout << test_params << '\n';
            std::cout << "at index: " << i << '\n';
            return false;
        }
        if (strcmp(password_buffer.data(), line.data()) != 0) {
            std::cout << "Wrong result for:\n";
            std::cout << test_params << '\n';
            std::cout << "at index: " << i << '\n';
            std::cout << "expected: " << line << '\n';
            std::cout << "recieved: " << password_buffer << '\n';
            return false;
        }
        i++;
    }
    if (i != password_generator->GetLength()) {
        std::cout << "wrong length for:\n";
        std::cout << test_params << '\n';
        std::cout << "reached index: " << i;
        std::cout << "expected: " << password_generator->GetLength() << '\n';
        return false;
    }
    return true;
}

TEST(PasswordGeneration, sanity) {
    ASSERT_TRUE(RunPassGenTest("sanity.txt"));
}

TEST(PasswordGeneration, dict) {
    ASSERT_TRUE(RunPassGenTest("dict.txt"));
}

TEST(PasswordGeneration, cartesian) {
    ASSERT_TRUE(RunPassGenTest("cartesian.txt"));
}

TEST(PasswordGeneration, or_) {
    ASSERT_TRUE(RunPassGenTest("or.txt"));
}