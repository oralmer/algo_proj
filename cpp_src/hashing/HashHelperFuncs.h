#pragma once

#include <string>
#include <vector>
#include <gmpxx.h>

std::vector<uint8_t> HexStringToVec(const std::string &hex_str);

std::vector<uint8_t> HMACpp(const std::vector<uint8_t> &key, const std::vector<uint8_t> &value);

std::vector<uint8_t> MpzToVector(const mpz_t x);