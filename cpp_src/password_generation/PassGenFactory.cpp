#include <iostream>
#include "PassGenFactory.h"
#include "CartesianPassGen.h"
#include "OrPassGen.h"
#include "CharPassGen.h"
#include "DictPassGen.h"

std::unique_ptr<IPassGen> PassGenFactory::BuildPassGen(const nlohmann::json &params) {
    std::cout << "fact\n";
    if(params[TYPE] == CARTESIAN){
        std::cout << "cart\n";
        return std::unique_ptr<IPassGen>(new CartesianPassGen(params));
    }
    if(params[TYPE] == OR){
        std::cout << "or\n";
        return std::unique_ptr<IPassGen>(new OrPassGen(params));
    }
    if(params[TYPE] == CHARACHTER){
        std::cout << "char\n";
        return std::unique_ptr<IPassGen>(new CharPassGen(params));
    }
    if(params[TYPE] == DICTIONARY){
        std::cout << "dict\n";
        return std::unique_ptr<IPassGen>(new DictPassGen(params));
    }
    std::cout << "err\n";
    throw std::runtime_error("not yet implemented");
}
