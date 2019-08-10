#pragma once

#include "CountTest.h"
#include "ThresholdHasher.h"
#include "WPA3KeyDeriver.h"
#include <gmpxx.h>

class ModPHasher : public ThresholdHasher<ModPHasher> {
    mpz_class m_P;
    std::unique_ptr<WPA3KeyDeriver> m_keyDeriver;
public:
    bool RunSingleTest(std::string password, const std::unique_ptr<CountTest> &test) const;

    explicit ModPHasher(nlohmann::json params);
};