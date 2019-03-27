#pragma once

#include "nlohmann/json.hpp"
#include "IPassGen.h"

class OrPassGen : public IPassGen {
private:
    size_t m_length;
    std::vector<std::unique_ptr<IPassGen>> m_sub_generators;
public:
    explicit OrPassGen(nlohmann::json params);

    size_t GetLength() const override;

    size_t GetMaxPassLength() const override;

    size_t operator()(char *password, size_t index) const override;
};
