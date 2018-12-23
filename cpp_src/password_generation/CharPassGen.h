#pragma once

#include "IPassGen.h"
#include "PassGenFactory.h"

class CharPassGen: public IPassGen {
    std::vector<char> m_characters;
public:
    explicit CharPassGen(PassGenParams);
    size_t GetLength() override;
    std::string operator()(size_t index) override;
};
