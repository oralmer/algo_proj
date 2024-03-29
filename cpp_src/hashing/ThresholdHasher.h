
#pragma once

#include <vector>
#include "CountTest.h"
#include "IHasher.h"

static const std::string P = "P";

template<class Tester>
class ThresholdHasher : public IHasher{
    std::vector<std::unique_ptr<CountTest>> m_tests;
    float m_cutoff;
public:
    bool operator()(std::string password) const override;

    explicit ThresholdHasher(nlohmann::json params);
};

#include "ThresholdHasher.inl"

