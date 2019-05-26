#pragma once

#include "IHasher.h"
#include <nlohmann/json.hpp>

static const std::string HASH_TYPE = "type";
static const std::string ELLIPTIC = "elliptic";
static const std::string MODP = "ModP";
static const std::string ID = "id";

class HasherFactory {
public:
    static std::unique_ptr<IHasher> BuildHasher(const nlohmann::json &params);
};
