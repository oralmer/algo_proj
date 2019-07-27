#pragma once

#include "ThresholdHasher.h"

template<class Tester>
bool ThresholdHasher<Tester>::operator()(std::string password) {
    int successes = 0;
    int failures = 0;
    for (const auto &test: m_tests) {
        if (static_cast<Tester *>(this)->RunSingleTest(password, test)) {
            successes++;
        } else {
            failures++;
        }
        if ((float) failures / m_tests.size() > 1 - m_cutoff) {
            return false;
        }
        if ((float) successes / m_tests.size() >= m_cutoff) {
            return true;
        }
    }
    return (float) successes / m_tests.size() >= m_cutoff;
}

template<class Tester>
ThresholdHasher<Tester>::ThresholdHasher(nlohmann::json params) : m_cutoff(params[CUTOFF].get<float>()) {
    //TODO: fix all gets
    for (const auto &test_params : params[TESTS]) {
        m_tests.push_back(std::make_unique<CountTest>(test_params));
    }
}
