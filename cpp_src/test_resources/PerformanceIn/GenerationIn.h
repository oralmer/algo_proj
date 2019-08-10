#pragma once

#include <string>

const std::string generation_pref_in = R"JSON({
    "type":"cartesian",
    "sub_generators":[
        {
            "type":"character",
            "characters":"1234567890"
        },
        {
            "type":"character",
            "characters":"1234567890"
        },
        {
            "type":"character",
            "characters":"1234567890"
        },
        {
            "type":"dictionary",
            "path":"perf_dict"
        },
        {
            "type":"dictionary",
            "path":"perf_dict"
        },
        {
            "type":"or",
            "sub_generators":
            [
                {
                    "type":"character",
                    "characters":"1234567890"
                },
                {
                    "type":"character",
                    "characters":"abcdefghi"
                },
                {
                    "type":"dictionary",
                    "path":"perf_dict"
                }
            ]
        }
    ]
})JSON";