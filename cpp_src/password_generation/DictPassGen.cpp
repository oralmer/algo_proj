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

size_t DictPassGen::GetLength() {
    return m_words.size();
}

std::string DictPassGen::operator()(size_t index) {
    return m_words[index];
}

std::string DictPassGen::get_current_dir() {
    char buffer[MAX_PATH];
    GetModuleFileName(nullptr, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos) + "\\";
}
