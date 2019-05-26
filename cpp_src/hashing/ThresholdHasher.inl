#pragma once
#include "ThresholdHasher.h"

template<class Tester>
bool ThresholdHasher<Tester>::operator()(std::string password) const {
    int total = 0;
    for (const auto &test: m_tests) {
        if (static_cast<const Tester *>(this)->RunSingleTest(password, test)) {
            total++;
        }
    }
    return (float) total / m_tests.size() >= m_cutoff;
}

template<class Tester>
ThresholdHasher<Tester>::ThresholdHasher(nlohmann::json params) : m_cutoff(params[CUTOFF].get<float>()) {
    //TODO: fix all gets
    for (const auto &test_params : params[TESTS]) {
        m_tests.push_back(std::make_unique<CountTest>(test_params));
    }
}
