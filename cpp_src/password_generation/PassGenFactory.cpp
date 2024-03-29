#include <iostream>
#include "PassGenFactory.h"
#include "CartesianPassGen.h"
#include "OrPassGen.h"
#include "CharPassGen.h"
#include "DictPassGen.h"
#include "BigDictPassGen.h"

std::unique_ptr<IPassGen> PassGenFactory::BuildPassGen(const nlohmann::json &params) {
    if(params[PASS_TYPE] == CARTESIAN){
        return std::unique_ptr<IPassGen>(new CartesianPassGen(params));
    }
    if(params[PASS_TYPE] == OR){
        return std::unique_ptr<IPassGen>(new OrPassGen(params));
    }
    if(params[PASS_TYPE] == CHARACTER){
        return std::unique_ptr<IPassGen>(new CharPassGen(params));
    }
    if(params[PASS_TYPE] == DICTIONARY){
        return std::unique_ptr<IPassGen>(new DictPassGen(params));
    }
    if(params[PASS_TYPE] == BIG_DICTIONARY){
        return std::unique_ptr<IPassGen>(new BigDictPassGen(params));
    }
    throw std::invalid_argument("not yet implemented");

    //TODO: have PG return iterators that start from index i and use them.
}
