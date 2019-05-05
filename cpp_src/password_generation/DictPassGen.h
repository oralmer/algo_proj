#pragma once

#include "IPassGen.h"
#include "PassGenFactory.h"
#include "nlohmann/json.hpp"

#define DICTS_PATH_ENV "DICTS_PATH"

class DictPassGen : public IPassGen {
    static std::string get_dicts_dir();

    std::vector<std::string> m_words;
public:
    explicit DictPassGen(nlohmann::json params);

    size_t GetMaxPassLength() const override;

    size_t GetLength() const override;

    size_t operator()(char *password, size_t index) const override;
};
