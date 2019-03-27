#pragma once

#include <optional>
#include "nlohmann/json.hpp"
#include "IPassGen.h"

class CartesianPassGen : public IPassGen {
//    std::optional<size_t> m_length;
    size_t m_length;
    std::vector<std::unique_ptr<IPassGen>> m_sub_generators;
public:
    explicit CartesianPassGen(nlohmann::json pramas);

    size_t GetLength() const override;

    size_t GetMaxPassLength() const override;

    size_t operator()(char *password, size_t index) const override;
};
