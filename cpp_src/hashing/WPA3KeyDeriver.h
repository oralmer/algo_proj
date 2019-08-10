#pragma once

#include <vector>
#include <gmpxx.h>
#include <openssl/hmac.h>

struct KeyDeriverCache {
    std::vector<uint8_t> key_vec;
    std::vector<uint8_t> curr_in;
    std::vector<uint8_t> seed;
    std::vector<uint8_t> result_piece;
    HMAC_CTX *hmac;
    int num_of_iters;
};

class WPA3KeyDeriver {
private:
    mutable KeyDeriverCache m_cache;
    std::vector<uint8_t> m_P_str;
    static const std::string LABEL;

    inline static const void PackShort(std::vector<uint8_t> &v, short srt, int offset);

    void HMACpp(const std::vector<uint8_t> &key, const std::vector<uint8_t> &value, uint8_t *hash) const;

public:
    explicit WPA3KeyDeriver(mpz_class P);

    std::unique_ptr<mpz_class> DeriveKey(const std::vector<uint8_t> &password, const std::vector<uint8_t> &hash) const;


};



