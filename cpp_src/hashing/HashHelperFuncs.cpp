#include <stdexcept>
#include <openssl/sha.h>
#include "HashHelperFuncs.h"

uint8_t CharToInt(char input) {
    if (input >= '0' && input <= '9')
        return static_cast<uint8_t>(input - '0');
    if (input >= 'A' && input <= 'F')
        return static_cast<uint8_t>(input - 'A' + 10);
    if (input >= 'a' && input <= 'f')
        return static_cast<uint8_t>(input - 'a' + 10);
    throw std::invalid_argument("Invalid input string");
}

std::vector<uint8_t> HexStringToVec(const std::string &hex_str) {
    auto arr = std::vector<uint8_t>();
    if (hex_str.length() % 2 != 0) {
        arr.resize(hex_str.length() / 2 + 1);
        arr[0] = CharToInt(hex_str[0]);
        for (int i = 1; i <= hex_str.length() / 2; ++i) {
            arr[i] = CharToInt(hex_str[2 * i - 1]) * 16 + CharToInt(hex_str[2 * i]);
        }
    } else {
        arr.resize(hex_str.length() / 2);
        for (int i = 0; i < hex_str.length() / 2; ++i) {
            arr[i] = CharToInt(hex_str[2 * i]) * 16 + CharToInt(hex_str[2 * i + 1]);
        }
    }
    return arr;
}

void HMACpp(const std::vector<uint8_t> &key, const std::vector<uint8_t> &value, uint8_t *hash, HMAC_CTX *hmac) {
    unsigned int len = SHA256_DIGEST_LENGTH;
    size_t keyLen = key.size();
    size_t valueLen = value.size();

    HMAC_Init_ex(hmac, key.data(), keyLen, EVP_sha256(), nullptr);
    HMAC_Update(hmac, value.data(), valueLen);
    HMAC_Final(hmac, hash, &len);
}

std::vector<uint8_t> MpzToVector(const mpz_t x) {
    size_t size = (mpz_sizeinbase(x, 2) + CHAR_BIT - 1) / CHAR_BIT;
    std::vector<uint8_t> v(size);
    mpz_export(&v[0], &size, 1, 1, 0, 0, x);
    v.resize(size);
    return v;
}