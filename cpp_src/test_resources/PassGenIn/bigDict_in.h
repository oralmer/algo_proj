#pragma once

#include <string>

// This test uses a cartesian to test the rewinding as well.
const std::string big_dict_in = R"JSON({
    "type":"cartesian",
    "sub_generators":[
        {
        "type":"big_dictionary",
        "path":"test_dict"
        },
        {
            "type":"character",
            "characters":"12"
        }
    ]
})JSON";