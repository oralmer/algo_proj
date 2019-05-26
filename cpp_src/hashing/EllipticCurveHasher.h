#pragma once

#include "CountTest.h"
#include "ThresholdHasher.h"
#include <gmpxx.h>

static const std::string A = "A";
static const std::string B = "B";

class EllipticCurveHasher : public ThresholdHasher<EllipticCurveHasher> {
    mpz_class m_P;
    mpz_class m_a;
    mpz_class m_b;
    static const std::string LABEL;

    std::unique_ptr<mpz_class> DeriveKey(const std::vector<uint8_t> &password, const std::vector<uint8_t> &hash) const;

    inline static const void PackShort(std::vector<uint8_t> &v, short srt);

public:
    bool RunSingleTest(const std::string &password, const std::unique_ptr<CountTest> &test) const;

    explicit EllipticCurveHasher(nlohmann::json params);
};
