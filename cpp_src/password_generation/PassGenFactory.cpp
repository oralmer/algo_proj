
#include <iostream>
#include "PassGenFactory.h"
#include "CartesianPassGen.h"
#include "OrPassGen.h"
#include "CharPassGen.h"
#include "DictPassGen.h"

std::unique_ptr<IPassGen> PassGenFactory::BuildPassGen(const nlohmann::json &params) {
    if(params[TYPE] == CARTESIAN){
        return std::unique_ptr<IPassGen>(new CartesianPassGen(params));
    }
    if(params[TYPE] == OR){
        return std::unique_ptr<IPassGen>(new OrPassGen(params));
    }
    if(params[TYPE] == CHARACHTER){
        return std::unique_ptr<IPassGen>(new CharPassGen(params));
    }
    if(params[TYPE] == DICTIONARY){
        return std::unique_ptr<IPassGen>(new DictPassGen(params));
    }
    throw std::runtime_error("not yet implemented");
}
