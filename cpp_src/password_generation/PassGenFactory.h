#pragma once

#include <vector>
#include <string>
#include <functional>
#include "IPassGen.h"

class PassGenFactory {
public:
    static std::unique_ptr<IPassGen> BuildPassGen(PassGenParams params);
};