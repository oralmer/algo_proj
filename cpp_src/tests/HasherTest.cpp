#include <gtest/gtest.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include "../hashing/EllipticCurveHasher.h"
#include "../test_resources/HasherIn/EChasher_in.h"
#include "../hashing/HasherFactory.h"

TEST(hashing, EllipticCurve){
    nlohmann::json test_params = nlohmann::json::parse(EChasher_in);
    std::unique_ptr<IHasher> hasher = HasherFactory::BuildHasher(test_params);
    ASSERT_TRUE((*hasher)(EChasher_pass));
    ASSERT_FALSE((*hasher)(EChasher_pass+"0"));
    ASSERT_FALSE((*hasher)("test"));
    ASSERT_FALSE((*hasher)("12345678"));
}