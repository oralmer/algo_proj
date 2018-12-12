
#pragma once

#include <string>
#include <tuple>

#define UNBOUNDED SIZE_MAX

enum PassType {
    chars, nums
};

struct PassLength {
    int length;
    int sum;
};

class PasswordGenerator {
    PassType m_pass_type;

    static std::string GenerateCartesian(size_t index, char zero_char, int char_num);

    static std::string GenerateCartesian(size_t index, char zero_char, int char_num, int length);

    static PassLength CalcLengthCartesian(size_t index, int base);

public:
    explicit PasswordGenerator(enum PassType pass_type);

    std::string operator()(size_t index);

    size_t IterSize();

};

