#pragma once

#include "CountTest.h"
#include "ThresholdHasher.h"
#include <gmpxx.h>
#include <openssl/hmac.h>

static const std::string A = "A";
static const std::string B = "B";

struct EllipticCurveCache {
    std::vector<uint8_t> key_vec;
    std::vector<uint8_t> curr_in;
    std::vector<uint8_t> seed;
    std::vector<uint8_t> result_piece;
    HMAC_CTX *hmac;
    int num_of_iters;
};

class EllipticCurveHasher : public ThresholdHasher<EllipticCurveHasher> {
    mpz_class m_P;
    mpz_class m_a;
    mpz_class m_b;
    mutable EllipticCurveCache m_cache;
    static const std::string LABEL;

    std::unique_ptr<mpz_class> DeriveKey(const std::vector<uint8_t> &password, const std::vector<uint8_t> &hash) const;

    inline static const void PackShort(std::vector<uint8_t> &v, short srt, int offset);

public:
    bool RunSingleTest(const std::string &password, const std::unique_ptr<CountTest> &test) const;

    explicit EllipticCurveHasher(nlohmann::json params);
};
