#include "CartesianPassGen.h"
#include "PassGenFactory.h"

size_t CartesianPassGen::GetLength() {
    if(m_length == 0){
        m_length = 1;
        for(auto&& gen: m_sub_generators){
            m_length *= gen->GetLength();
        }
    }
    return m_length;
}

std::string CartesianPassGen::operator()(size_t index) {
    if(index > GetLength()){
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
    m_length = 0;
    for(nlohmann::json& sub_params: pramas[SUB_GEN]){
        m_sub_generators.push_back(PassGenFactory::BuildPassGen(sub_params));
    }
}
