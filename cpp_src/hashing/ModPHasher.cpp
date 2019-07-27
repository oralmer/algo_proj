#include "ModPHasher.h"

bool ModPHasher::RunSingleTest(std::string password, const std::unique_ptr<CountTest> &test) {
    int counter = 0;
    mpz_class value;
    do {
        counter++;
        if (counter > test->m_count) {
            return false;
        }
        //value = DeriveKey(password, test->m_MAC1, test->m_MAC2, test->m_count);
    } while (value > m_P);
    return counter == test->m_count;
}


ModPHasher::ModPHasher(nlohmann::json params) : ThresholdHasher(params) {
    m_P = (std::string) params[P];
}
