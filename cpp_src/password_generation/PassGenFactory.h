#pragma once

#include <vector>
#include <string>
#include <functional>
#include <nlohmann/json.hpp>
#include "IPassGen.h"

class PassGenFactory {
public:
    static std::unique_ptr<IPassGen> BuildPassGen(const nlohmann::json &params);
};