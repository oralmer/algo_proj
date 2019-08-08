#pragma once

#include <fstream>
#include <nlohmann/json.hpp>
#include "IPassGen.h"

class BigDictPassGen : public IPassGen {
    mutable std::string m_string_chache;
    mutable size_t m_curr_index;
    mutable std::ifstream m_dict_file;
    size_t m_length;
    size_t m_maxPassLength;

    void ResetFile() const;

    void GotToInd(int ind) const;

public:
    static const int MAX_WORD_SIZE = 200;

    explicit BigDictPassGen(nlohmann::json params);

    size_t GetMaxPassLength() const override;

    size_t GetLength() const override;

    size_t operator()(char *password, size_t index) const override;
};



