#pragma once

#include "IPassGen.h"
#include "PassGenFactory.h"
#include "nlohmann/json.hpp"

class CharPassGen : public IPassGen {
    std::vector<char> m_characters;
public:
    explicit CharPassGen(nlohmann::json params);

    size_t GetLength() const override;

    size_t operator()(char *password, size_t index) const override;

    size_t GetMaxPassLength() const override;
};
