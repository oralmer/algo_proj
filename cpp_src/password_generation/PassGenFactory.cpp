
#include "PassGenFactory.h"
#include "CartesianPassGen.h"
#include "OrPassGen.h"
#include "CharPassGen.h"

std::unique_ptr<IPassGen> PassGenFactory::BuildPassGen(PassGenParams params) {
    switch(params.m_part_type){
        case cartesian:
            return std::unique_ptr<IPassGen>(new CartesianPassGen(params));
        case or_:
            return std::unique_ptr<IPassGen>(new OrPassGen(params));
        case character:
            return std::unique_ptr<IPassGen>(new CharPassGen(params));
        default:
            throw std::runtime_error("not yet implemented");
    }
}
