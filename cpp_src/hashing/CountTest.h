#pragma once

#include <nlohmann/json.hpp>
#include <gmpxx.h>

#define MAC1 "MAC1"
#define MAC2 "MAC2"
#define COUNT "count"

class CountTest {
public:
    std::string m_hash;
    int m_count;

    explicit CountTest(nlohmann::json test_params);
};
