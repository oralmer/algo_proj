#include "CartesianPassGen.h"
#include "PassGenFactory.h"

size_t CartesianPassGen::GetLength() {
    return m_length;
}

std::string CartesianPassGen::operator()(size_t index) {
    if(index > m_length){
        throw std::runtime_error("index out of range in CartesianPassGen");
    }
    std::string password;
    for(const auto& gen: m_sub_generators){
        password += (*gen)(index % gen->GetLength());
        index /= gen->GetLength();
    }
    return password;
}

CartesianPassGen::CartesianPassGen(nlohmann::json pramas) {
    for(nlohmann::json& sub_params: pramas[SUB_GEN]){
        m_sub_generators.push_back(PassGenFactory::BuildPassGen(sub_params));
    }
    m_length = 1;
    for(auto&& gen: m_sub_generators){
        m_length *= gen->GetLength();
    }
}
