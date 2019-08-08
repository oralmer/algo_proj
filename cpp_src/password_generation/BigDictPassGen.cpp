#include <iostream>
#include <windows.h>
#include "BigDictPassGen.h"
#include "GenerationHelperFuncs.h"


BigDictPassGen::BigDictPassGen(nlohmann::json params) : m_curr_index(0) {
    std::string dict_path = params[PATH];
    m_maxPassLength = 0;
    m_length = 0;
    m_dict_file = std::ifstream(GetDictsDir() + "/" + dict_path);
    while (getline(m_dict_file, m_string_chache)) {
        m_string_chache.erase(m_string_chache.find_last_not_of(" \n\r\t") + 1); //remove trailing whitespace
        m_length++;
        m_maxPassLength = std::max(m_maxPassLength, m_string_chache.length());
    }
    ResetFile();
}

size_t BigDictPassGen::GetLength() const {
    return m_length;
}

size_t BigDictPassGen::operator()(char *password, size_t index) const {
    if (index > m_length) {
        throw std::invalid_argument("index out of range in DictPassGen");
    }
    if (index != m_curr_index + 1) {
        GotToInd(index); //While expensive, this happens very rarely, 1/<size of dict>, so it is still amortized O(1)
    }
    m_curr_index = index;
    getline(m_dict_file, m_string_chache);
    m_string_chache.erase(m_string_chache.find_last_not_of(" \n\r\t") + 1);
    strcpy(password, m_string_chache.c_str());
    return m_string_chache.length();
}

size_t BigDictPassGen::GetMaxPassLength() const {
    return m_maxPassLength;

}

void BigDictPassGen::ResetFile() const {
    m_dict_file.clear();
    m_dict_file.seekg(0, std::ios::beg);
}

void BigDictPassGen::GotToInd(int ind) const {
    ResetFile();
    for (int i = 0; i < ind; ++i) {
        m_dict_file.ignore(MAX_WORD_SIZE, '\n');
    }
}


