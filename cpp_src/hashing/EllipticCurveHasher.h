#pragma once

#include "CountTest.h"
#include "ThresholdHasher.h"
#include <gmpxx.h>


class EllipticCurveHasher : public ThresholdHasher<EllipticCurveHasher> {
    mpz_class m_P;
    mpz_class m_a;
    mpz_class m_b;
public:
    bool RunSingleTest(std::string password, const std::unique_ptr<CountTest> &test) const;

    explicit EllipticCurveHasher(nlohmann::json params);
};
