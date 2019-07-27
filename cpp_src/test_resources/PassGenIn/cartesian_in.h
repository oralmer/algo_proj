#pragma once

#include <string>

const std::string cartesian_in = R"JSON({
    "type":"cartesian",
    "sub_generators":[
        {
            "type":"character",
            "characters":"123"
        },
        {
            "type":"character",
            "characters":"abc"
        }
    ]
})JSON";
