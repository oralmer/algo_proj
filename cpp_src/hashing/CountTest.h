#pragma once

#include <string>
#include <nlohmann/json.hpp>

#define MAC1 "MAC1"
#define MAC2 "MAC2"
#define COUNT "count"

class CountTest {
public:
    std::string m_MAC1;
    std::string m_MAC2;
    int m_count;

    explicit CountTest(nlohmann::json test_params);
};
