#pragma once

#include <string>
#include <vector>

enum PassPartType {
    cartesian,
    or_,
    dict,
    character
};

struct PassGenParams {
    PassPartType m_part_type;
    std::vector<PassGenParams> m_sub_parts;
    std::string m_additional_data;
};


class IPassGen {
public:
    virtual size_t GetLength() = 0;
    virtual std::string operator()(size_t index) = 0;
};
