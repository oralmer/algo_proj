

#include <windows.h>
#include <iostream>
#include <fstream>
#include "GenerationHelperFuncs.h"

std::string GetDictsDir() {
    const char* dict_env = getenv(DICTS_PATH_ENV.c_str());
    return std::string(dict_env);
}