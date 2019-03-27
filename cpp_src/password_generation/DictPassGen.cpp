#include <fstream>
#include <iostream>
#include <windows.h>
#include "DictPassGen.h"

static const std::string dicts_dir = R"(C:\Users\or\PycharmProjects\algo_proj\algo_proj\cpp_src\dictionaries\)";

DictPassGen::DictPassGen(nlohmann::json params) { //TODO: fix
    m_words = std::vector<std::string>();
    std::string dict_path = params[PATH];
    std::string line;
//    std::ifstream dict_file(R"(C:\Users\or\PycharmProjects\algo_proj\algo_proj\cpp_src\dictionaries\test_dict)");
    std::ifstream dict_file(dicts_dir + dict_path);
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
        throw "index out of range in DictPassGen";
    }
    strcpy(password,  m_words[index].c_str());
    return m_words[index].length();
}

std::string DictPassGen::get_current_dir() { //TODO: move dicts to relative dir
//    char buffer[MAX_PATH];
//    GetModuleFileName(nullptr, buffer, MAX_PATH);
//    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
//    return std::string(buffer).substr(0, pos) + "\\";
    return R"(C:\Users\or\PycharmProjects\algo_proj\algo_proj\cpp_src\password_generation)";
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
