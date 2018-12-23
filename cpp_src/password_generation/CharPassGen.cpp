#include "CharPassGen.h"

CharPassGen::CharPassGen(PassGenParams params) {
    m_characters = std::vector<char>(params.m_additional_data.begin(), params.m_additional_data.end());
}

size_t CharPassGen::GetLength() {
    return m_characters.size();
}

std::string CharPassGen::operator()(size_t index) {
    return std::string(1, m_characters[index]);
}
