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
            return gen->operator()(index);
        }
        index -= gen->GetLength();
    }
}

OrPassGen::OrPassGen(PassGenParams params) {
    m_length = 0;
    for(PassGenParams& sub_params: params.m_sub_parts){
        m_sub_generators.push_back(PassGenFactory::BuildPassGen(sub_params));
    }
}
