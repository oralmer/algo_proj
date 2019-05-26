#include "HasherFactory.h"
#include "IDHasher.h"
#include "EllipticCurveHasher.h"
#include "ModPHasher.h"

std::unique_ptr<IHasher> HasherFactory::BuildHasher(const nlohmann::json &params) {
    if(params[HASH_TYPE] == ELLIPTIC){
        return std::unique_ptr<IHasher>(new EllipticCurveHasher(params));
    }
    if(params[HASH_TYPE] == MODP){
        return std::unique_ptr<IHasher>(new ModPHasher(params));
    }
    if(params[HASH_TYPE] == ID){
        return std::unique_ptr<IHasher>(new IDHasher(params));
    }
    throw std::invalid_argument("not yet implemented");
}
