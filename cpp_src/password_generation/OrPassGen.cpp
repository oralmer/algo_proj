#include "OrPassGen.h"
#include "PassGenFactory.h"

size_t OrPassGen::GetLength() const{
    return m_length;
}

size_t OrPassGen::operator()(char *password, size_t index) const{
    if (index > m_length) {
        throw std::runtime_error("index out of range in OrPassGen");
    }
    size_t result = 0;
    for (const auto &gen: m_sub_generators) {
        if (index < gen->GetLength()) {
            result = (*gen)(password, index);
            break;
        }
        index -= gen->GetLength();
    }
    return result;
}

OrPassGen::OrPassGen(nlohmann::json params) {
    for (nlohmann::json &sub_params: params[SUB_GEN]) {
        m_sub_generators.push_back(PassGenFactory::BuildPassGen(sub_params));
    }
    m_length = 0;
    for (const auto &gen: m_sub_generators) {
        m_length += gen->GetLength();
    }
}

size_t OrPassGen::GetMaxPassLength() const {
    size_t maxLen = 0;
    for (const auto &gen: m_sub_generators){
        if(gen->GetMaxPassLength() > maxLen){
            maxLen = gen->GetMaxPassLength();
        }
    }
    return maxLen;
}
