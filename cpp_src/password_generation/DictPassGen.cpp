#include <fstream>
#include <iostream>
#include <windows.h>
#include "DictPassGen.h"
#include "GenerationHelperFuncs.h"


DictPassGen::DictPassGen(nlohmann::json params) {
    m_words = std::vector<std::string>();
    std::string dict_path = params[PATH];
    std::string line;
    std::ifstream dict_file(GetDictsDir() + "/" + dict_path);
    while (getline(dict_file, line)) {
        line.erase(line.find_last_not_of(" \n\r\t")+1); //remove trailing whitespace
        m_words.push_back(line);
    }
}

size_t DictPassGen::GetLength() const {
    return m_words.size();
}

size_t DictPassGen::operator()(char *password, size_t index) const {
    if (index > m_words.size()) {
        throw std::invalid_argument("index out of range in DictPassGen");
    }
    strcpy(password,  m_words[index].c_str());
    return m_words[index].length();
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
