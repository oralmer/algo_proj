#pragma once

#include <string>
#include <vector>
#include <gmpxx.h>
#include <openssl/hmac.h>

std::vector<uint8_t> HexStringToVec(const std::string &hex_str);

std::vector<uint8_t> MpzToVector(const mpz_t x);