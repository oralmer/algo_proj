#pragma once

#include "CountTest.h"
#include "ThresholdHasher.h"
#include <gmpxx.h>

class ModPHasher : public ThresholdHasher<ModPHasher> {
    mpz_class m_P;
public:
    bool RunSingleTest(std::string password, const std::unique_ptr<CountTest> &test);

    explicit ModPHasher(nlohmann::json params);
};