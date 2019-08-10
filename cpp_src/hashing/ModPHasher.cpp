#include "ModPHasher.h"

bool ModPHasher::RunSingleTest(std::string password, const std::unique_ptr<CountTest> &test) const {
    int counter = 0;
    std::unique_ptr<mpz_class> value;
    std::vector<uint8_t> pass_vec(password.length() + 1);
    std::copy(password.begin(), password.end(), pass_vec.begin());
    size_t pass_len = password.length();
    do {
        counter++;
        if (counter > test->m_count) {
            return false;
        }
        pass_vec[pass_len] = static_cast<uint8_t>(counter);
        value = m_keyDeriver->DeriveKey(pass_vec, test->m_hash);
    } while ((*value) > m_P);
    return counter == test->m_count;
}


ModPHasher::ModPHasher(nlohmann::json params) : ThresholdHasher(params) {
    m_P = (std::string) params[P];
    m_keyDeriver = std::make_unique<WPA3KeyDeriver>(m_P);
}
