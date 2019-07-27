#include "EllipticCurveHasher.h"
#include "HashHelperFuncs.h"
#include <openssl/sha.h>

const std::string EllipticCurveHasher::LABEL = "SAE Hunting and Pecking";

bool EllipticCurveHasher::RunSingleTest(const std::string &password, const std::unique_ptr<CountTest> &test) {
    int counter = 0;
    mpz_class power, residue;
    mpz_class exp(3);
    std::vector<uint8_t> pass_vec(password.length() + 1);
    std::copy(password.begin(), password.end(), pass_vec.begin());
    size_t pass_len = password.length();
    do {
        counter++;
        if (counter > test->m_count) {
            return false;
        }
        pass_vec[pass_len] = static_cast<uint8_t>(counter);
        auto value = this->DeriveKey(pass_vec, test->m_hash);
        if ((*value) > m_P) {
            continue;
        }
        mpz_powm(power.get_mpz_t(), value->get_mpz_t(), exp.get_mpz_t(), m_P.get_mpz_t());
        residue = power + m_a * (*value) + m_b;
    } while (mpz_legendre(residue.get_mpz_t(), m_P.get_mpz_t()) != 1);
    return counter == test->m_count;
}


EllipticCurveHasher::EllipticCurveHasher(nlohmann::json params) : ThresholdHasher(params) {
    m_P = mpz_class(params[P].get<std::string>());
    assert(mpz_sizeinbase(m_P.get_mpz_t(), 2) % 8 == 0);
    m_a = mpz_class(params[A].get<std::string>());
    m_b = mpz_class(params[B].get<std::string>());

    auto P_str = MpzToVector(m_P.get_mpz_t());
    auto length = static_cast<short>(mpz_sizeinbase(m_P.get_mpz_t(), 2));
    m_num_of_iters = static_cast<size_t>( std::ceil(length / 256.0));
    m_key_vec = std::vector<uint8_t>(SHA256_DIGEST_LENGTH * m_num_of_iters);
    m_curr_in = std::vector<uint8_t>(4 + LABEL.length() + P_str.size());
    m_seed = std::vector<uint8_t>(SHA256_DIGEST_LENGTH);
    m_result_piece = std::vector<uint8_t>(SHA256_DIGEST_LENGTH);

    // set constant parts of curr_in
    // curr_in structure: [i, LABEL, P_str, length]
    auto it = m_curr_in.begin() + 2;
    it = std::copy(LABEL.begin(), LABEL.end(), it);
    std::copy(P_str.begin(), P_str.end(), it);
    PackShort(m_curr_in, length, 2 + LABEL.size() + P_str.size());

    m_hmac = HMAC_CTX_new();
}

std::unique_ptr<mpz_class>
EllipticCurveHasher::DeriveKey(const std::vector<uint8_t> &password, const std::vector<uint8_t> &hash) {
    HMACpp(hash, password, m_seed.data(), m_hmac);
    auto res_it = m_key_vec.begin();
    // since P, length and LABEL are always the same, we can update just i - the loop does this for us.
    auto i = (short *) m_curr_in.data();
    for ((*i) = 1; (*i) <= m_num_of_iters; ++(*i)) {
        HMACpp(m_seed, m_curr_in, m_result_piece.data(), m_hmac);
        res_it = std::copy(m_result_piece.begin(), m_result_piece.end(), res_it);
    }
    auto result = std::make_unique<mpz_class>();
    //turn the binary string in m_key_vec into an mpz
    mpz_import(result->get_mpz_t(), m_key_vec.size(), 1, 1, 0, 0, m_key_vec.data());
    return result;
}

inline const void EllipticCurveHasher::PackShort(std::vector<uint8_t> &v, short srt, int offset) {
    v[0 + offset] = static_cast<uint8_t>(srt); //short -> uint8_t takes lowest byte
    v[1 + offset] = static_cast<uint8_t>(srt >> 8);
}
//TODO: opencl