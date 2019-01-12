#include "CharPassGen.h"

CharPassGen::CharPassGen(nlohmann::json params) {
    std::string chars = params[CHARACHTERS];
    m_characters = std::vector<char>(chars.begin(), chars.end());
}

size_t CharPassGen::GetLength() {
    return m_characters.size();
}

std::string CharPassGen::operator()(size_t index) {
    return std::string(1, m_characters[index]);
}
