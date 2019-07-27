#pragma once

#include "CountTest.h"
#include "ThresholdHasher.h"
#include <gmpxx.h>
#include <openssl/hmac.h>

static const std::string A = "A";
static const std::string B = "B";

class EllipticCurveHasher : public ThresholdHasher<EllipticCurveHasher> {
    mpz_class m_P;
    mpz_class m_a;
    mpz_class m_b;
    std::vector<uint8_t> m_key_vec;
    std::vector<uint8_t> m_curr_in;
    std::vector<uint8_t> m_seed;
    std::vector<uint8_t> m_result_piece;
    HMAC_CTX *m_hmac;
    int m_num_of_iters;
    static const std::string LABEL;

    std::unique_ptr<mpz_class> DeriveKey(const std::vector<uint8_t> &password, const std::vector<uint8_t> &hash);

    inline static const void PackShort(std::vector<uint8_t> &v, short srt, int offset);

public:
    bool RunSingleTest(const std::string &password, const std::unique_ptr<CountTest> &test);

    explicit EllipticCurveHasher(nlohmann::json params);
};
