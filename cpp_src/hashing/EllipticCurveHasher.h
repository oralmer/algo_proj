#pragma once

#include "CountTest.h"
#include "ThresholdHasher.h"
#include <gmpxx.h>

#define A_js "a"
#define B_js "b"

class EllipticCurveHasher : public ThresholdHasher<EllipticCurveHasher> {
    mpz_class m_P;
    mpz_class m_a;
    mpz_class m_b;
    static const std::string LABEL;

    mpz_class DeriveKey(const std::string &password, const std::string &hash, int counter) const;

    inline static const void pack_short(std::stringstream &s, short srt);

public:
    bool RunSingleTest(const std::string &password, const std::unique_ptr<CountTest> &test) const;

    explicit EllipticCurveHasher(nlohmann::json params);
};
