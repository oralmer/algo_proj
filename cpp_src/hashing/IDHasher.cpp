#include "IDHasher.h"

bool IDHasher::operator()(std::string password) const {
    return password == m_password;
}

IDHasher::IDHasher(nlohmann::json params) : m_password(params[PASSWORD]) {}
