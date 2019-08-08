#pragma once

#include <nlohmann/json.hpp>
#include "IHasher.h"

class IDHasher : public IHasher {
    std::string m_password;
public:
    bool operator()(std::string password) const override;

    explicit IDHasher(nlohmann::json params);
};
