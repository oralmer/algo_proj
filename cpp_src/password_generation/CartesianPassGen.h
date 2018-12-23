#pragma once

#include "IPassGen.h"

class CartesianPassGen: public  IPassGen{
    size_t m_length;
    std::vector<std::unique_ptr<IPassGen>> m_sub_generators;
public:
    explicit CartesianPassGen(PassGenParams params);
    size_t GetLength() override;
    std::string operator()(size_t index) override;
};
