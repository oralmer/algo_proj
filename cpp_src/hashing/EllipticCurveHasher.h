#pragma once

#include <gmpxx.h>
#include <openssl/hmac.h>
#include "CountTest.h"
#include "ThresholdHasher.h"
#include "WPA3KeyDeriver.h"

static const std::string A = "A";
static const std::string B = "B";

class EllipticCurveHasher : public ThresholdHasher<EllipticCurveHasher> {
    mpz_class m_P;
    mpz_class m_a;
    mpz_class m_b;
    std::unique_ptr<WPA3KeyDeriver> m_keyDeriver;
public:
    bool RunSingleTest(const std::string &password, const std::unique_ptr<CountTest> &test) const;

    explicit EllipticCurveHasher(nlohmann::json params);
};
