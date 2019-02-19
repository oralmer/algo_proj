#include "CharPassGen.h"

CharPassGen::CharPassGen(nlohmann::json params) {
    std::string chars = params[CHARACHTERS];
    m_characters = std::vector<char>(chars.begin(), chars.end());
}

size_t CharPassGen::GetLength() const{
    return m_characters.size();
}

size_t CharPassGen::operator()(char *password, size_t index) const{
    if(index > m_characters.size()){
        throw std::runtime_error("index out of range in CharPassGen");
    }
    password[0] = m_characters[index];
    password[1] = '\0';
    return 1;
}

size_t CharPassGen::GetMaxPassLength() const {
    return 1;
}
