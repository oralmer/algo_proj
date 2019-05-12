
#include "ThresholdHasher.h"
#include "EllipticCurveHasher.h"
#include "ModPHasher.h"

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
ThresholdHasher<Tester>::ThresholdHasher(nlohmann::json params) {
    m_cutoff = params[CUTOFF];
    for (const auto &test_params : params[TESTS]) {
        m_tests.push_back(std::make_unique<CountTest>(test_params));
    }
}

//TODO: is this the correct solution? https://stackoverflow.com/questions/8752837/undefined-reference-to-template-class-constructor
template class ThresholdHasher<EllipticCurveHasher>;
template class ThresholdHasher<ModPHasher>;