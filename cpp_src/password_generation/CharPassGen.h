#pragma once

#include "IPassGen.h"
#include "PassGenFactory.h"
#include "nlohmann/json.hpp"

class CharPassGen: public IPassGen {
    std::vector<char> m_characters;
public:
    explicit CharPassGen(nlohmann::json params);
    size_t GetLength() override;
    std::string operator()(size_t index) override;
};
