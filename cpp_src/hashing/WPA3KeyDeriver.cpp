#include <memory>
#include <openssl/sha.h>
#include <cmath>
#include "WPA3KeyDeriver.h"
#include "HashHelperFuncs.h"


const std::string WPA3KeyDeriver::LABEL = "SAE Hunting and Pecking";

std::unique_ptr<mpz_class>
WPA3KeyDeriver::DeriveKey(const std::vector<uint8_t> &password, const std::vector<uint8_t> &hash) const {
    HMACpp(hash, password, m_cache.seed.data());
    auto res_it = m_cache.key_vec.begin();
    // since P, length and LABEL are always the same, we can update just i - the loop does this for us.
    auto i = (short *) m_cache.curr_in.data();
    for ((*i) = 1; (*i) <= m_cache.num_of_iters; ++(*i)) {
        HMACpp(m_cache.seed, m_cache.curr_in, m_cache.result_piece.data());
        res_it = std::copy(m_cache.result_piece.begin(), m_cache.result_piece.end(), res_it);
    }
    auto result = std::make_unique<mpz_class>();
    //turn the binary string in m_key_vec into an mpz
    mpz_import(result->get_mpz_t(), m_cache.key_vec.size(), 1, 1, 0, 0, m_cache.key_vec.data());
    return result;
}

inline const void WPA3KeyDeriver::PackShort(std::vector<uint8_t> &v, short srt, int offset) {
    v[0 + offset] = static_cast<uint8_t>(srt); //short -> uint8_t takes lowest byte
    v[1 + offset] = static_cast<uint8_t>(srt >> 8);
}

WPA3KeyDeriver::WPA3KeyDeriver(mpz_class P) {
    m_P_str = MpzToVector(P.get_mpz_t());
    auto length = static_cast<short>(mpz_sizeinbase(P.get_mpz_t(), 2));
    m_cache.num_of_iters = static_cast<size_t>( std::ceil(length / 256.0));
    m_cache.key_vec = std::vector<uint8_t>(SHA256_DIGEST_LENGTH * m_cache.num_of_iters);
    m_cache.curr_in = std::vector<uint8_t>(4 + LABEL.length() + m_P_str.size());
    m_cache.seed = std::vector<uint8_t>(SHA256_DIGEST_LENGTH);
    m_cache.result_piece = std::vector<uint8_t>(SHA256_DIGEST_LENGTH);

    // set constant parts of curr_in
    // curr_in structure: [i, LABEL, P_str, length]
    auto it = m_cache.curr_in.begin() + 2;
    it = std::copy(LABEL.begin(), LABEL.end(), it);
    std::copy(m_P_str.begin(), m_P_str.end(), it);
    PackShort(m_cache.curr_in, length, 2 + LABEL.size() + m_P_str.size());

    m_cache.hmac = HMAC_CTX_new();
}

void WPA3KeyDeriver::HMACpp(const std::vector<uint8_t> &key, const std::vector<uint8_t> &value, uint8_t *hash) const {
    unsigned int len = SHA256_DIGEST_LENGTH;
    size_t keyLen = key.size();
    size_t valueLen = value.size();

    HMAC_Init_ex(m_cache.hmac, key.data(), keyLen, EVP_sha256(), nullptr);
    HMAC_Update(m_cache.hmac, value.data(), valueLen);
    HMAC_Final(m_cache.hmac, hash, &len);
}