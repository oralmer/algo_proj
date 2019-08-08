#pragma once

#include <nlohmann/json.hpp>
#include <gmpxx.h>

static const std::string MAC1 = "MAC1";
static const std::string MAC2 = "MAC2";
static const std::string COUNT = "count";

class CountTest {
public:
    std::vector<uint8_t> m_hash;
    int m_count;

    explicit CountTest(nlohmann::json test_params);
};
