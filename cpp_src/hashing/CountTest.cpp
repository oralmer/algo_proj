
#include "CountTest.h"

CountTest::CountTest(nlohmann::json test_params) :
        m_MAC1(test_params[MAC1]),
        m_MAC2(test_params[MAC2]),
        m_count(test_params[COUNT]) {}
