#include <fstream>
#include <iostream>
#include <windows.h>
#include "DictPassGen.h"


DictPassGen::DictPassGen(nlohmann::json params) {
    m_words = std::vector<std::string>();
    std::string dict_path = params[PATH];
    std::string line;
    std::ifstream dict_file(get_current_dir() + dict_path);
    while (getline(dict_file, line)) {
        m_words.push_back(line);
    }
}

size_t DictPassGen::GetLength() const {
    return m_words.size();
}

size_t DictPassGen::operator()(char *password, size_t index) const {
    if (index > m_words.size()) {
        throw std::runtime_error("index out of range in DictPassGen");
    }
    strcpy_s(password, m_words[index].length() + 1, m_words[index].c_str());
    return m_words[index].length();
}

std::string DictPassGen::get_current_dir() {
    char buffer[MAX_PATH];
    GetModuleFileName(nullptr, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos) + "\\";
}

size_t DictPassGen::GetMaxPassLength() const {
    size_t maxLen = 0;
    for (const auto &word: m_words){
        if(word.size() > maxLen){
            maxLen = word.size();
        }
    }
    return maxLen;

}
