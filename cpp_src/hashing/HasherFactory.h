#pragma once

#include "IHasher.h"
#include <nlohmann/json.hpp>

#define TYPE "type"
#define ELLIPTIC "elliptic"
#define MODP "ModP"
#define ID "id"

class HasherFactory {
public:
    static std::unique_ptr<IHasher> BuildHasher(const nlohmann::json &params);
};
