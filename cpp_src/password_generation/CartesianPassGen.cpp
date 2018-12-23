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
    for(auto&& gen: m_sub_generators){
        password += gen->operator()(index % gen->GetLength());
        index /= gen->GetLength();
    }
    return password;
}

CartesianPassGen::CartesianPassGen(PassGenParams params) {
    m_length = 0;
    for(PassGenParams& sub_params: params.m_sub_parts){
        m_sub_generators.push_back(PassGenFactory::BuildPassGen(sub_params));
    }
}
