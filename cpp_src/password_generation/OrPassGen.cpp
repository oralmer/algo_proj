#include "OrPassGen.h"
#include "PassGenFactory.h"

size_t OrPassGen::GetLength() {
    if(m_length == 0){
        m_length = 0;
        for(auto&& gen: m_sub_generators){
            m_length += gen->GetLength();
        }
    }
    return m_length;
}

std::string OrPassGen::operator()(size_t index) {
    if(index > GetLength()){
        throw std::runtime_error("index out of range in CartesianPassGen");
    }
    for(auto&& gen: m_sub_generators){
        if(index < gen->GetLength()){
            return (*gen)(index);
        }
        index -= gen->GetLength();
    }
}

OrPassGen::OrPassGen(nlohmann::json params) {
    m_length = 0;
    for(nlohmann::json& sub_params: params[SUB_GEN]){
        m_sub_generators.push_back(PassGenFactory::BuildPassGen(sub_params));
    }
}
