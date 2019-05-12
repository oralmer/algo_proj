#include "EllipticCurveHasher.h"
#include <sstream>
//#include <openssl>

const std::string EllipticCurveHasher::LABEL = "SAE Hunting and Pecking";

bool EllipticCurveHasher::RunSingleTest(const std::string &password, const std::unique_ptr<CountTest> &test) const {
    int counter = 0;
    mpz_class value, power, residue;
    mpz_class exp(3);
    do {
        counter++;
        if (counter > test->m_count) {
            return false;
        }
        value = this->DeriveKey(password, test->m_hash, counter);
        if(value > m_P){
            continue;
        }
        mpz_powm(power.get_mpz_t(), value.get_mpz_t(), exp.get_mpz_t(), m_P.get_mpz_t());
        residue = power + m_a * value + m_b;
    } while (mpz_legendre(residue.get_mpz_t(), m_P.get_mpz_t()) != 1);
    return counter == test->m_count;
}


EllipticCurveHasher::EllipticCurveHasher(nlohmann::json params) : ThresholdHasher(params) {
    m_P = (std::string) params[P];
    m_a = (std::string) params[A_js];
    m_b = (std::string) params[B_js];
    assert(mpz_sizeinbase(m_P.get_mpz_t(), 2) % 8 == 0);
}

mpz_class EllipticCurveHasher::DeriveKey(const std::string &password, const std::string &hash, int counter) const {
    std::string seed; // = HMAC(password, password + (char)counter);
    std::string result;
    auto length = static_cast<short>(mpz_sizeinbase(m_P.get_mpz_t(), 2));
    auto iter_num = static_cast<size_t>( std::ceil(length / 256.0));
    auto P_str = m_P.get_str(16);
    for (short i = 1; i <= iter_num; ++i) {
        std::stringstream curr_in;
        EllipticCurveHasher::pack_short(curr_in, i);
        curr_in << LABEL;
        curr_in << P_str;
        EllipticCurveHasher::pack_short(curr_in, length);
        result += curr_in.str();//HMAC(seed, curr_in.str())
    }
    return mpz_class("0x" + result);
}

inline const void EllipticCurveHasher::pack_short(std::stringstream &s, short srt) {
    s << static_cast<char>(srt % 256);
    s << static_cast<char>(srt / 256);
}