#include "EllipticCurveHasher.h"
#include "HashHelperFuncs.h"
#include <openssl/sha.h>

const std::string EllipticCurveHasher::LABEL = "SAE Hunting and Pecking";

bool EllipticCurveHasher::RunSingleTest(const std::string &password, const std::unique_ptr<CountTest> &test) const {
    int counter = 0;
    mpz_class power, residue;
    mpz_class exp(3);
    std::vector<uint8_t> pass_vec(password.begin(), password.end());
    size_t pass_len = password.length();
    pass_vec.resize(pass_len + 1);
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
    m_a = mpz_class(params[A].get<std::string>());
    m_b = mpz_class(params[B].get<std::string>());
    assert(mpz_sizeinbase(m_P.get_mpz_t(), 2) % 8 == 0);
}

std::unique_ptr<mpz_class>
EllipticCurveHasher::DeriveKey(const std::vector<uint8_t> &password, const std::vector<uint8_t> &hash) const {
    auto length = static_cast<short>(mpz_sizeinbase(m_P.get_mpz_t(), 2));
    auto P_str = MpzToVector(m_P.get_mpz_t());
    auto num_of_iters = static_cast<size_t>( std::ceil(length / 256.0));

    std::vector<uint8_t> seed = HMACpp(hash, password);
    std::vector<uint8_t> curr_in(4 + LABEL.length() + P_str.size());
    std::vector<uint8_t> short_vec(2);
    std::vector<uint8_t> result_piece;
    std::vector<uint8_t> result_vec(SHA256_DIGEST_LENGTH * num_of_iters);
    auto res_it = result_vec.begin();

    for (short i = 1; i <= num_of_iters; ++i) {
        auto it = curr_in.begin();
        EllipticCurveHasher::PackShort(short_vec, i);
        it = std::copy(short_vec.begin(), short_vec.end(), it);
        it = std::copy(LABEL.begin(), LABEL.end(), it);
        it = std::copy(P_str.begin(), P_str.end(), it);
        EllipticCurveHasher::PackShort(short_vec, length);
        std::copy(short_vec.begin(), short_vec.end(), it);

        result_piece = HMACpp(seed, curr_in);
        res_it = std::copy(result_piece.begin(), result_piece.end(), res_it);
    }
    auto result = std::make_unique<mpz_class>();
    mpz_import(result->get_mpz_t(), result_vec.size(), 1, 1, 0, 0, &result_vec[0]);
    return result;
}

inline const void EllipticCurveHasher::PackShort(std::vector<uint8_t> &v, short srt) {
    v[0] = static_cast<uint8_t>(srt % 256);
    v[1] = static_cast<uint8_t>(srt / 256);
}
//TODO: opencl