
#include "CountTest.h"
#include <algorithm>

CountTest::CountTest(nlohmann::json test_params) : m_count(test_params[COUNT]) {
    std::string mac1 = test_params[MAC1];
    mac1.erase(std::remove(mac1.begin(), mac1.end(), ':'), mac1.end());
    std::string mac2 = test_params[MAC2];
    mac2.erase(std::remove(mac2.begin(), mac2.end(), ':'), mac2.end());
    m_hash = mac1 > mac2 ? mac1 + mac2 : mac2 + mac1;
}
