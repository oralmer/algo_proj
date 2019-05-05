#include "EllipticCurveHasher.h"

//TODO: wrap gmp

bool EllipticCurveHasher::RunSingleTest(std::string password, const std::unique_ptr<CountTest> &test) const {
    int counter = 0;
    mpz_class value, power, residue;
    mpz_class exp(3);
    do {
        counter++;
        if (counter > test->m_count) {
            return false;
        }
        //value = DeriveKey(password, test->m_MAC1, test->m_MAC2, counter);
        mpz_powm(power.get_mpz_t(), value.get_mpz_t(), exp.get_mpz_t(), m_P.get_mpz_t());
        residue = power + m_a*value + m_b;
    } while (mpz_legendre(residue.get_mpz_t(), m_P.get_mpz_t()) != 1);
    return counter == test->m_count;
}


EllipticCurveHasher::EllipticCurveHasher(nlohmann::json params) : ThresholdHasher(params){
    m_P = (std::string) params[P];
    m_a = (std::string) params[A];
    m_b = (std::string) params[B];
}
