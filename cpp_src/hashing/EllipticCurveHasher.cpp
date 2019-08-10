#include "EllipticCurveHasher.h"
#include "HashHelperFuncs.h"
#include <openssl/sha.h>


bool EllipticCurveHasher::RunSingleTest(const std::string &password, const std::unique_ptr<CountTest> &test) const {
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
        auto value = m_keyDeriver->DeriveKey(pass_vec, test->m_hash);
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
    m_keyDeriver = std::make_unique<WPA3KeyDeriver>(m_P);
}