#include "CartesianPassGen.h"
#include "PassGenFactory.h"

size_t CartesianPassGen::GetLength() const {
    return m_length;
}

size_t CartesianPassGen::operator()(char *password, size_t index) const {
    if (index > m_length) {
        throw std::invalid_argument("index out of range in CartesianPassGen");
    }
    size_t location = 0;
    for (const auto &gen: m_sub_generators) {
        location += (*gen)(&(password[location]), index % gen->GetLength());
        index /= gen->GetLength();
    }
    return location;
}

CartesianPassGen::CartesianPassGen(nlohmann::json pramas) {
    for (nlohmann::json &sub_params: pramas[SUB_GEN]) {
        m_sub_generators.push_back(PassGenFactory::BuildPassGen(sub_params));
    }
    m_length = 1;
    for (const auto &gen: m_sub_generators) {
        m_length *= gen->GetLength();
    }
}

size_t CartesianPassGen::GetMaxPassLength() const {
    size_t len = 0;
    for (const auto &gen: m_sub_generators){
        len += gen->GetMaxPassLength();
    }
    return len;
}
