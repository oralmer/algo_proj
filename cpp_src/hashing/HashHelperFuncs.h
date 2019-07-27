#pragma once

#include <string>
#include <vector>
#include <gmpxx.h>
#include <openssl/hmac.h>

std::vector<uint8_t> HexStringToVec(const std::string &hex_str);

void HMACpp(const std::vector<uint8_t> &key, const std::vector<uint8_t> &value, uint8_t *hash, HMAC_CTX *hmac);

std::vector<uint8_t> MpzToVector(const mpz_t x);