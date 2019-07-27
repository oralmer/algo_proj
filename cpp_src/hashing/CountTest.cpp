
#include "CountTest.h"
#include <algorithm>
#include "HashHelperFuncs.h"

CountTest::CountTest(nlohmann::json test_params) : m_count(test_params[COUNT].get<int>()) {
    auto mac1 = test_params[MAC1].get<std::string>();
    mac1.erase(std::remove(mac1.begin(), mac1.end(), ':'), mac1.end());
    auto mac2 = test_params[MAC2].get<std::string>();
    mac2.erase(std::remove(mac2.begin(), mac2.end(), ':'), mac2.end());
    if(mac1 < mac2){
        mac1.swap(mac2);
    }
    m_hash = HexStringToVec(mac1 + mac2);
}
